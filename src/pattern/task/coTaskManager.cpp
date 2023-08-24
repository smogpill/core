// Copyright(c) 2019-2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coTaskManager.h"
#include "math/scalar/coAtomicInt32_f.h"
#include "lang/result/coResult_f.h"
#include <debug/profiler/coProfile.h>
#include <math/scalar/coUint32_f.h>
#include <container/queue/coDynamicQueue_f.h>
#include "../thread/coThread_f.h"
#include "../lock/coScopedLock.h"
#include "coTask.h"

coDEFINE_SINGLETON(coTaskManager);

coTaskManager::coTaskManager(coInt nbThreads)
{
	coASSERT(instance == nullptr);
	instance = this;
	static_assert(coIsPowerOf2(s_queueSize));

	if (nbThreads < 0)
		nbThreads = coGetMaxConcurrentThreads() - 1;

	StartThreads(nbThreads);
}

coTaskManager::~coTaskManager()
{
	StopThreads();

	for (coTaskBarrier* barrier : _barriers)
		delete barrier;
	instance = nullptr;
}

void coTaskManager::StartThreads(coUint nb)
{
	// If no threads are requested we're done
	if (nb == 0)
		return;

	_exit = false;

	for (coUint priorityIdx = 0; priorityIdx < coUint(coTaskPriority::END); ++priorityIdx)
	{
		Queue& queue = _queues[priorityIdx];
		coASSERT(queue._heads == nullptr);
		queue._heads = new std::atomic<coUint32>[nb];
		for (coUint i = 0; i < nb; ++i)
			queue._heads[i] = 0;

		coASSERT(queue._threads.count == 0);
		coReserve(queue._threads, nb);
		const coTaskPriority priority = static_cast<coTaskPriority>(priorityIdx);
		for (coUint threadIdx = 0; threadIdx < nb; ++threadIdx)
			coPushBack(queue._threads, new std::thread([this, priority, threadIdx] { WorkerThreadMain(priority, threadIdx); }));
	}
}

coUint coTaskManager::GetBestHead(const Queue& queue) const
{
	// Find the minimal value across all threads
	coUint head = queue._tail;
	for (coUint i = 0; i < queue._threads.count; ++i)
		head = coMin(head, queue._heads[i].load());
	return head;
}

void coTaskManager::StopThreads()
{
	_exit = true;

	for (Queue& queue : _queues)
	{
		if (queue._threads.count == 0)
			return;

		queue._semaphore.Release(queue._threads.count);

		for (std::thread* thread : queue._threads)
		{
			if (thread->joinable())
				thread->join();
			delete thread;
		}

		coClear(queue._threads);

		for (coUint32 head = 0; head != queue._tail; ++head)
		{
			coTask* task = queue._queuedTasks[head & (s_queueSize - 1)].exchange(nullptr);
			if (task)
			{
				task->Execute();
				task->RemoveRef();
			}
		}

		delete[] queue._heads;
		queue._heads = nullptr;
		queue._tail = 0;
	}
}

void coTaskManager::SetNbThreads(coInt nbRawThreads)
{
	const coUint nbThreads = nbRawThreads < 0 ? (std::thread::hardware_concurrency() - 1) : nbRawThreads;
	if (_queues[0]._threads.count == nbThreads)
		return;
	StopThreads();
	StartThreads(nbThreads);
}

coTaskHandle coTaskManager::CreateTask(const coTaskFunction& function, const coTaskSetup& setup)
{
	coTask* task = new coTask();
	task->SetFunction(function);
	task->SetSetup(setup);
	return coTaskHandle(task);
}

coTaskBarrier* coTaskManager::CreateBarrier()
{
	// Find the first unused barrier
	for (coTaskBarrier* barrier : _barriers)
	{
		coBool expected = false;
		if (barrier->_inUse.compare_exchange_strong(expected, true))
			return barrier;
	}

	return nullptr;
}

void coTaskManager::DestroyBarrier(coTaskBarrier& barrier)
{
	coASSERT(barrier.IsEmpty());
	coBool expected = true;
	barrier._inUse.compare_exchange_strong(expected, false);
	coASSERT(expected);
}

void coTaskManager::WaitForTasks(coTaskBarrier& barrier)
{
	barrier.Wait();
}

void coTaskManager::QueueTask(coTask& task)
{
	QueueTaskInternal(task);
	_queues[coUint(task.GetPriority())]._semaphore.Release();
}

void coTaskManager::FreeTask(coTask& task)
{
	delete &task;
}

void coTaskManager::QueueTasks(coTask** tasks, coUint nb)
{
	coASSERT(nb > 0);
	coUint prioNbs[coUint(coTaskPriority::END)] = {};
	for (coTask** task = tasks, **taskEnd = tasks + nb; task < taskEnd; ++task)
	{
		++prioNbs[coUint((**task).GetPriority())];
		QueueTaskInternal(**task);
	}
	for (coUint priority = 0; priority < coUint(coTaskPriority::END); ++priority)
	{
		const coUint prioNb = prioNbs[priority];
		if (prioNb)
		{
			Queue& queue = _queues[priority];
			queue._semaphore.Release(coMin(prioNb, queue._threads.count));
		}
	}
}

void coTaskManager::QueueTaskInternal(coTask& task)
{
	// Add reference to job because we're adding the job to the queue
	task.AddRef();

	Queue& queue = _queues[coUint(task.GetPriority())];

	// Need to read head first because otherwise the tail can already have passed the head
	// We read the head outside of the loop since it involves iterating over all threads and we only need to update
	// it if there's not enough space in the queue.
	coUint head = GetBestHead(queue);

	for (;;)
	{
		// Check if there's space in the queue
		coUint oldValue = queue._tail;
		if (oldValue - head >= s_queueSize)
		{
			// We calculated the head outside of the loop, update head (and we also need to update tail to prevent it from passing head)
			head = GetBestHead(queue);
			oldValue = queue._tail;

			// Second check if there's space in the queue
			if (oldValue - head >= s_queueSize)
			{
				// Wake up all threads in order to ensure that they can clear any nullptrs they may not have processed yet
				queue._semaphore.Release(queue._threads.count);

				// Sleep a little (we have to wait for other threads to update their head pointer in order for us to be able to continue)
				std::this_thread::sleep_for(std::chrono::microseconds(100));
				continue;
			}
		}

		// Write the job pointer if the slot is empty
		coTask* expectedTask = nullptr;
		const coBool success = queue._queuedTasks[oldValue & (s_queueSize - 1)].compare_exchange_strong(expectedTask, &task);

		// Regardless of who wrote the slot, we will update the tail (if the successful thread got scheduled out 
		// after writing the pointer we still want to be able to continue)
		queue._tail.compare_exchange_strong(oldValue, oldValue + 1);

		// If we successfully added our job we're done
		if (success)
			break;
	}
}

void coTaskManager::WorkerThreadMain(coTaskPriority priority, coUint threadIdx)
{
	// Thread name
	coDynamicString threadName;
	{
		threadName = "TaskWorker_";
		switch (priority)
		{
		case coTaskPriority::FRAME: threadName << "Frame"; break;
		case coTaskPriority::BACKGROUND: threadName << "Background"; break;
		default:
		{
			coASSERT(false);
			break;
		}
		}
		threadName << "_";
		threadName << threadIdx;
		coNullTerminate(threadName);
		coSetThreadName(threadName.data);
	}
	
	// Thread priority
	{
		coThreadPriority threadPriority = coThreadPriority::NORMAL;
		switch (priority)
		{
		case coTaskPriority::FRAME: threadPriority = coThreadPriority::NORMAL; break;
		case coTaskPriority::BACKGROUND: threadPriority = coThreadPriority::LOW; break;
		default: coASSERT(false); break;
		}
		coSetThreadPriority(threadPriority);
	}

	coPROFILE_THREAD(threadName.data);

	Queue& queue = _queues[coUint(priority)];
	std::atomic<coUint32>& head = queue._heads[threadIdx];

	while (!_exit)
	{
		queue._semaphore.Acquire();

		// Execute tasks
		while (head != queue._tail)
		{
			// Exchange any job pointer we find with a nullptr
			std::atomic<coTask*>& taskAtomic = queue._queuedTasks[head & (s_queueSize - 1)];
			if (taskAtomic.load())
			{
				coTask* task = taskAtomic.exchange(nullptr);
				if (task)
				{
					task->Execute();
					task->RemoveRef();
				}
			}
			++head;
		}
	}
}
