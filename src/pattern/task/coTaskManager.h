// Copyright(c) 2019-2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coTask.h"
#include "pattern/lock/coMutex.h"
#include "pattern/lock/coCountingSemaphore.h"
#include "pattern/singleton/coSingleton.h"
#include "container/array/coDynamicArray.h"
#include "container/queue/coDynamicQueue.h"
#include "memory/coCache.h"
#include "coTaskHandle.h"
class coTaskBarrier;

// Inspired by Jorrit Rouwe's Jolt JobSystemThreadPool

class coTaskManager
{
	coDECLARE_SINGLETON(coTaskManager);
public:
	coTaskManager(coInt nbThreads = -1);
	~coTaskManager();

	void SetNbThreads(coInt nbThreads);
	coTaskHandle CreateTask(const std::function<void()>& function, const coTaskSetup& setup);
	coTaskBarrier* CreateBarrier();
	void DestroyBarrier(coTaskBarrier& barrier);
	void WaitForTasks(coTaskBarrier& barrier);

private:
	static constexpr coUint s_queueSize = 4096;
	struct Queue
	{
		std::atomic<coTask*> _queuedTasks[s_queueSize] = {};
		std::atomic<coUint32>* _heads = nullptr;
		alignas(coCACHE_LINE_SIZE) std::atomic<coUint32> _tail = 0;
		coDynamicArray<std::thread*> _threads;
		coCountingSemaphore32 _semaphore;
	};

	friend class coTask;
	friend class coTaskHandle;
	void QueueTask(coTask& task);
	void FreeTask(coTask& task);
	void QueueTasks(coTask** tasks, coUint nb);
	void QueueTaskInternal(coTask& task);
	void WorkerThreadMain(coTaskPriority priority, coUint threadIdx);
	void StopThreads();
	void StartThreads(coUint nb);
	coUint GetBestHead(const Queue& queue) const;

	Queue _queues[coUint(coTaskPriority::END)];
	coDynamicArray<coTaskBarrier*> _barriers;
	std::atomic<coBool> _exit = false;
};
