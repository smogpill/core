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
	static_assert(coIsPowerOf2(s_queueSize));

	if (nbThreads < 0)
		nbThreads = coGetMaxConcurrentThreads();

	coReserve(_threads, nbThreads);
	for (coUint threadIdx = 0; threadIdx < coUint(nbThreads); ++threadIdx)
		coPushBack(_threads, new std::thread([this, threadIdx] { WorkerThreadMain(threadIdx); }));
}

coTaskSystem::~coTaskSystem()
{
	exit = true;
	_semaphore.Release(_threads.count);
	for (std::thread* thread : _threads)
	{
		if (thread->joinable())
			thread->join();
		delete thread;
	}
}

void coTaskSystem::QueueTask(coTask& task)
{
	QueueTaskInternal(task);
	_semaphore.Release();
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
	coUint head = GetHead();

	for (;;)
	{
		// Check if there's space in the queue
		coUint oldValue = _tail;
		if (oldValue - head >= s_queueSize)
		{
			// We calculated the head outside of the loop, update head (and we also need to update tail to prevent it from passing head)
			head = GetHead();
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
	coDynamicString threadName = coDynamicString("TaskWorker") + threadIdx;
	coSetThreadName(threadName);
	coPROFILE_THREAD(threadName);

	std::atomic<coUint32>& head = _heads[threadIdx];

	while (!exit)
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
					task->Release();
				}
			}
			++head;
		}
	}
}
