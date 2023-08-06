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

// Inspired by Jorrit Rouwe's Jolt JobSystemThreadPool

class coTaskSystem
{
	coDECLARE_SINGLETON(coTaskSystem);
public:
	coTaskSystem(coInt nbThreads = -1);
	~coTaskSystem();

	void SetNbThreads(coInt nbThreads);

private:
	friend class coTask;
	void QueueTask(coTask& task);
	void FreeTask(coTask& task);
	void QueueTasks(coTask** tasks, coUint nb);
	void QueueTaskInternal(coTask& task);
	void WorkerThreadMain(coUint threadIdx);
	void StopThreads();
	void StartThreads(coUint nb);

	static constexpr coUint s_queueSize = 1024;
	coTask* _queuedTasks[s_queueSize] = {};
	std::atomic<coUint32>* _heads = nullptr;
	alignas(coCACHE_LINE_SIZE) std::atomic<coUint32> _tail = 0;
	coDynamicArray<std::thread*> _threads;
	coCountingSemaphore32 _semaphore;
	mutable coMutex _mutex;
	std::atomic<coBool> _exit = false;
};
