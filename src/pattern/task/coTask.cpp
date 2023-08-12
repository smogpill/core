// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coTask.h"
#include "coTaskManager.h"

coFORCE_INLINE coTaskManager& coTask::GetSystem() const
{
	coTaskManager* system = coTaskManager::instance;
	coASSERT(system);
	return *system;
}

void coTask::RemoveDependencyAndQueue(coUint nb)
{
	if (RemoveDependency(nb))
		GetSystem().QueueTask(*this);
}

void coTask::RemoveRef()
{
	// Releasing a reference must use release semantics...
	if (_nbRefs.fetch_sub(1, std::memory_order_release) == 1)
	{
		// ... so that we can use aquire to ensure that we see any updates from other threads that released a ref before freeing the job
		std::atomic_thread_fence(std::memory_order_acquire);
		GetSystem().FreeTask(*this);
	}
}
