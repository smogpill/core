// Copyright(c) 2019-2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <debug/profiler/coProfile.h>
#include <debug/log/coAssert.h>
#include "coTaskBarrier.h"

inline coUint32 coTask::Execute()
{
	// Transition job to executing state
	coUint32 state = 0; // We can only start running with a dependency counter of 0
	if (!_nbDependencies.compare_exchange_strong(state, s_executingState, std::memory_order_acquire))
		return state; // state is updated by compare_exchange_strong to the current value

	// Run the job function
	{
		//coPROFILE(mJobName, mColor.GetUInt32());
		_function();
	}

	// Fetch the barrier pointer and exchange it for the done state, so we're sure that no barrier gets set after we want to call the callback
	coIntPtr barrier = _barrier.load(std::memory_order_relaxed);
	for (;;)
	{
		if (_barrier.compare_exchange_weak(barrier, s_barrierDoneState, std::memory_order_relaxed))
			break;
	}
	coASSERT(barrier != s_barrierDoneState);

	// Mark job as done
	state = s_executingState;
	_nbDependencies.compare_exchange_strong(state, s_doneState, std::memory_order_relaxed);
	coASSERT(state == s_executingState);

	// Notify the barrier after we've changed the job to the done state so that any thread reading the state after receiving the callback will see that the job has finished
	if (barrier != 0)
		reinterpret_cast<coTaskBarrier*>(barrier)->OnTaskFinished();

	return s_doneState;
}

inline coBool coTask::SetBarrier(coTaskBarrier* barrier_)
{
	coIntPtr barrier = 0;
	if (_barrier.compare_exchange_strong(barrier, reinterpret_cast<intptr_t>(barrier_), std::memory_order_relaxed))
		return true;
	coASSERT(barrier == s_barrierDoneState); // A job can only belong to 1 barrier
	return false;
}

inline void coTask::AddDependency(coUint nb)
{
	const coUint32 oldValue = _nbDependencies.fetch_add(nb, std::memory_order_relaxed);
	coASSERT(oldValue > 0 && oldValue != s_executingState && oldValue != s_doneState); // Task is queued, running or done, it is not allowed to add a dependency to a running task.
}

inline coBool coTask::RemoveDependency(coUint nb)
{
	const coUint32 oldValue = _nbDependencies.fetch_sub(nb, std::memory_order_release);
	coASSERT(oldValue != s_executingState && oldValue != s_doneState); // Task is running or done, it is not allowed to add a dependency to a running task.
	coUint32 newValue = oldValue - nb;
	coASSERT(oldValue > newValue); // Test wrap around, this is a logic error.
	return newValue == 0;
}

inline void coTask::AddRef()
{
	// Adding a reference can use relaxed memory ordering
	_nbRefs.fetch_add(1, std::memory_order_relaxed);
}
