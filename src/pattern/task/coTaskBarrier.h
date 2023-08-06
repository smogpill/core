// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <memory/coCache.h>
#include "../lock/coCountingSemaphore.h"

// Based on Jorrit Rouwe's Jolt JobSystemThreadPool

class coTask;
class coTaskHandle;

class coTaskBarrier
{
public:
	coTaskBarrier();
	~coTaskBarrier();

	void AddTask(const coTaskHandle& handle);
	void AddTasks(const coTaskHandle* handles, coUint nbHandles);
	coBool IsEmpty() const { return _taskReadIndex == _taskWriteIndex; }
	void Wait();

	std::atomic<coBool> _inUse = false;

private:
	friend class coTask;

	void OnTaskFinished();

	static constexpr coUint s_maxNbJobs = 2048;
	std::atomic<coTask*> _tasks[s_maxNbJobs] = {};
	alignas(coCACHE_LINE_SIZE) std::atomic<coUint32> _taskReadIndex = 0;
	alignas(coCACHE_LINE_SIZE) std::atomic<coUint32> _taskWriteIndex = 0;
	std::atomic<coUint>	_nbToAcquire = 0;
	coCountingSemaphore32 _semaphore;
};
