// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coTaskHandle.h"
#include "coTaskManager.h"

void coTaskHandle::RemoveDependencies(coTaskHandle* handles, coUint nbHandles, coUint count)
{
	coASSERT(nbHandles > 0);

	coTaskManager* system = coTaskManager::instance;

	// Should prefer a dynamic stack allocation instead
	coASSERT(nbHandles < 2048);
	coTask* tasksToQueue[2048];
	coTask** nextTask = tasksToQueue;

	// Remove the dependencies on all jobs
	for (const coTaskHandle* handle = handles, *handleEnd = handles + nbHandles; handle < handleEnd; ++handle)
	{
		coTask* task = handle->GetPtr();
		if (task->RemoveDependency(count))
			*(nextTask++) = task;
	}

	// If any jobs need to be scheduled, schedule them as a batch
	const coUint nbJobsToQueue = coUint(nextTask - tasksToQueue);
	if (nbJobsToQueue)
		system->QueueTasks(tasksToQueue, nbJobsToQueue);
}
