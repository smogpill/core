// Copyright(c) 2019-2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coTaskSystem.h"
#include "math/scalar/coAtomicInt32_f.h"
#include "lang/result/coResult_f.h"
#include <debug/profiler/coProfile.h>
#include <math/scalar/coUint32_f.h>
#include <container/queue/coDynamicQueue_f.h>
#include "../thread/coThread_f.h"
#include "../lock/coScopedLock.h"
#include "coTask.h"

coDEFINE_SINGLETON(coTaskSystem);

coTaskSystem::coTaskSystem(coInt nbThreads)
{
	coASSERT(instance == nullptr);
	instance = this;
	static_assert(coIsPowerOf2(s_queueSize));

	if (nbThreads < 0)
		nbThreads = coGetMaxConcurrentThreads() - 1;

	StartThreads(nbThreads);
}

coTaskSystem::~coTaskSystem()
{
	StopThreads();

	for (coTaskBarrier* barrier : _barriers)
		delete barrier;
	instance = nullptr;
}

void coTaskSystem::StartThreads(coUint nb)
{
	// If no threads are requested we're done
	if (nb == 0)
		return;

	_exit = false;

	coASSERT(_heads == nullptr);
	_heads = new std::atomic<coUint32>[nb];
	for (coUint i = 0; i < nb; ++i)
		_heads[i] = 0;

	coASSERT(_threads.count == 0);
	coReserve(_threads, nb);
	for (coUint threadIdx = 0; threadIdx < nb; ++threadIdx)
		coPushBack(_threads, new std::thread([this, threadIdx] { WorkerThreadMain(threadIdx); }));
}

coUint coTaskSystem::GetBestHead() const
{
	// Find the minimal value across all threads
	coUint head = _tail;
	for (coUint i = 0; i < _threads.count; ++i)
		head = coMin(head, _heads[i].load());
	return head;
}

void coTaskSystem::StopThreads()
{
	if (_threads.count == 0)
		return;

	_exit = true;
	_semaphore.Release(_threads.count);

	for (std::thread* thread : _threads)
	{
		if (thread->joinable())
			thread->join();
		delete thread;
	}

	coClear(_threads);

	for (coUint32 head = 0; head != _tail; ++head)
	{
		coTask* task = _queuedTasks[head & (s_queueSize - 1)].exchange(nullptr);
		if (task)
		{
			task->Execute();
			task->RemoveRef();
		}
	}

	delete[] _heads;
	_heads = nullptr;
	_tail = 0;
}

void coTaskSystem::SetNbThreads(coInt nbRawThreads)
{
	const coUint nbThreads = nbRawThreads < 0 ? (std::thread::hardware_concurrency() - 1) : nbRawThreads;
	if (_threads.count == nbThreads)
		return;
	StopThreads();
	StartThreads(nbThreads);
}

coTaskHandle coTaskSystem::CreateTask(const std::function<void()>& func)
{
	coTask* task = new coTask();
	task->SetFunction(func);
	return coTaskHandle(task);
}

coTaskBarrier* coTaskSystem::CreateBarrier()
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

void coTaskSystem::DestroyBarrier(coTaskBarrier& barrier)
{
	coASSERT(barrier.IsEmpty());
	coBool expected = true;
	barrier._inUse.compare_exchange_strong(expected, false);
	coASSERT(expected);
}

void coTaskSystem::WaitForTasks(coTaskBarrier& barrier)
{
	barrier.Wait();
}

void coTaskSystem::QueueTask(coTask& task)
{
	QueueTaskInternal(task);
	_semaphore.Release();
}

void coTaskSystem::FreeTask(coTask& task)
{
	delete &task;
}

void coTaskSystem::QueueTasks(coTask** tasks, coUint nb)
{
	coASSERT(nb > 0);
	for (coTask** task = tasks, **taskEnd = tasks + nb; task < taskEnd; ++task)
		QueueTaskInternal(**task);
	_semaphore.Release(coMin(nb, _threads.count));
}

void coTaskSystem::QueueTaskInternal(coTask& task)
{
	// Add reference to job because we're adding the job to the queue
	task.AddRef();

	// Need to read head first because otherwise the tail can already have passed the head
	// We read the head outside of the loop since it involves iterating over all threads and we only need to update
	// it if there's not enough space in the queue.
	coUint head = GetBestHead();

	for (;;)
	{
		// Check if there's space in the queue
		coUint oldValue = _tail;
		if (oldValue - head >= s_queueSize)
		{
			// We calculated the head outside of the loop, update head (and we also need to update tail to prevent it from passing head)
			head = GetBestHead();
			oldValue = _tail;

			// Second check if there's space in the queue
			if (oldValue - head >= s_queueSize)
			{
				// Wake up all threads in order to ensure that they can clear any nullptrs they may not have processed yet
				_semaphore.Release(_threads.count);

				// Sleep a little (we have to wait for other threads to update their head pointer in order for us to be able to continue)
				std::this_thread::sleep_for(std::chrono::microseconds(100));
				continue;
			}
		}

		// Write the job pointer if the slot is empty
		coTask* expectedTask = nullptr;
		const coBool success = _queuedTasks[oldValue & (s_queueSize - 1)].compare_exchange_strong(expectedTask, &task);

		// Regardless of who wrote the slot, we will update the tail (if the successful thread got scheduled out 
		// after writing the pointer we still want to be able to continue)
		_tail.compare_exchange_strong(oldValue, oldValue + 1);

		// If we successfully added our job we're done
		if (success)
			break;
	}
}

void coTaskSystem::WorkerThreadMain(coUint threadIdx)
{
	coDynamicString threadName = "TaskWorker";
	threadName << threadIdx;
	coNullTerminate(threadName);
	coSetThreadName(threadName.data);
	coPROFILE_THREAD(threadName.data);

	std::atomic<coUint32>& head = _heads[threadIdx];

	while (!_exit)
	{
		_semaphore.Acquire();

		// Execute tasks
		while (head != _tail)
		{
			// Exchange any job pointer we find with a nullptr
			std::atomic<coTask*>& taskAtomic = _queuedTasks[head & (s_queueSize - 1)];
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
