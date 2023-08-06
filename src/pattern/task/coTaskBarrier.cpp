// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coTaskBarrier.h"
#include <math/scalar/coUint32_f.h>
#include <debug/profiler/coProfile.h>
#include "coTask.h"
#include "coTaskHandle.h"

// Based on Jorrit Rouwe's Jolt JobSystemThreadPool

coTaskBarrier::coTaskBarrier()
{
	static_assert(coIsPowerOf2(coTaskBarrier::s_maxNbJobs));
}

coTaskBarrier::~coTaskBarrier()
{
	coASSERT(IsEmpty());
}

void coTaskBarrier::AddTask(const coTaskHandle& taskHandle)
{
	coBool releaseSemaphore = false;

	// Set the barrier on the task, this returns true if the barrier was successfully set (otherwise the task is already done and we don't need to add it to our list)
	coTask* task = taskHandle.GetPtr();
	if (task->SetBarrier(this))
	{
		// If the task can be executed we want to release the semaphore an extra time to allow the waiting thread to start executing it
		++_nbToAcquire;
		if (task->CanBeExecuted())
		{
			releaseSemaphore = true;
			++_nbToAcquire;
		}

		// Add the task to our task list
		task->AddRef();
		coUint writeIndex = _taskWriteIndex++;
		while (writeIndex - _taskReadIndex >= s_maxNbJobs)
		{
			coASSERT(false); // Barrier full, stalling!
			std::this_thread::sleep_for(std::chrono::microseconds(100));
		}
		_tasks[writeIndex & (s_maxNbJobs - 1)] = task;
	}

	// Notify waiting thread that a new executable task is available
	if (releaseSemaphore)
		_semaphore.Release();
}

void coTaskBarrier::AddTasks(const coTaskHandle* handles, coUint nbHandles)
{
	coBool releaseSemaphore = false;

	for (const coTaskHandle* handle = handles, *handlesEnd = handles + nbHandles; handle < handlesEnd; ++handle)
	{
		// Set the barrier on the job, this returns true if the barrier was successfully set (otherwise the job is already done and we don't need to add it to our list)
		coTask* task = handle->GetPtr();
		if (task->SetBarrier(this))
		{
			// If the job can be executed we want to release the semaphore an extra time to allow the waiting thread to start executing it
			++_nbToAcquire;
			if (!releaseSemaphore && task->CanBeExecuted())
			{
				releaseSemaphore = true;
				++_nbToAcquire;
			}

			// Add the job to our job list
			task->AddRef();
			uint write_index = _taskWriteIndex++;
			while (write_index - _taskReadIndex >= s_maxNbJobs)
			{
				coASSERT(false); // Barrier full, stalling!
				std::this_thread::sleep_for(std::chrono::microseconds(100));
			}
			_tasks[write_index & (s_maxNbJobs - 1)] = task;
		}
	}

	// Notify waiting thread that a new executable job is available
	if (releaseSemaphore)
		_semaphore.Release();
}

void coTaskBarrier::Wait()
{
	while (_nbToAcquire > 0)
	{
		{
			coPROFILE_EVENT("Execute Jobs");

			// Go through all jobs
			bool has_executed;
			do
			{
				has_executed = false;

				// Loop through the jobs and erase jobs from the beginning of the list that are done
				while (_taskReadIndex < _taskWriteIndex)
				{
					std::atomic<coTask*>& task = _tasks[_taskReadIndex & (s_maxNbJobs - 1)];
					coTask* taskPtr = task.load();
					if (taskPtr == nullptr || !taskPtr->IsDone())
						break;

					// Job is finished, release it
					taskPtr->Release();
					task = nullptr;
					++_taskReadIndex;
				}

				// Loop through the jobs and execute the first executable job
				for (coUint index = _taskReadIndex; index < _taskWriteIndex; ++index)
				{
					const std::atomic<coTask*>& task = _tasks[index & (s_maxNbJobs - 1)];
					coTask* taskPtr = task.load();
					if (taskPtr != nullptr && taskPtr->CanBeExecuted())
					{
						// This will only execute the job if it has not already executed
						taskPtr->Execute();
						has_executed = true;
						break;
					}
				}

			} while (has_executed);
		}

		// Wait for another thread to wake us when either there is more work to do or when all jobs have completed
		const coInt32 nbToAcquire = coMax(1, _semaphore.GetValue()); // When there have been multiple releases, we acquire them all at the same time to avoid needlessly spinning on executing jobs
		_semaphore.Acquire(nbToAcquire);
		_nbToAcquire -= nbToAcquire;
	}

	// All jobs should be done now, release them
	while (_taskReadIndex < _taskWriteIndex)
	{
		std::atomic<coTask*>& task = _tasks[_taskReadIndex & (s_maxNbJobs - 1)];
		coTask* taskPtr = task.load();
		coASSERT(taskPtr != nullptr && taskPtr->IsDone());
		taskPtr->Release();
		task = nullptr;
		++_taskReadIndex;
	}
}

void coTaskBarrier::OnTaskFinished()
{
	_semaphore.Release();
}
