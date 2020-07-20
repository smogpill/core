// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/thread/coTaskScheduler.h"
#include "pattern/thread/coTaskWorkerThread.h"
#include "pattern/thread/coTaskContext.h"
#include "pattern/thread/coTask.h"
#include "pattern/thread/coThread_f.h"
#include "math/scalar/coAtomicInt32_f.h"
#include "lang/result/coResult_f.h"
#include <math/scalar/coUint32_f.h>
#include <container/queue/coDynamicQueue_f.h>

coDEFINE_SINGLETON(coTaskScheduler);

coTaskScheduler::coTaskScheduler(coUint nbWorkers)
{
	nbWorkers = coMin(nbWorkers, coGetMaxConcurrentThreads());
	for (coUint i = 0; i < nbWorkers; ++i)
	{
		coPushBack(workers, new coTaskWorkerThread(*this, i));
	}
}

coTaskScheduler::~coTaskScheduler()
{
	Stop();
	for (auto* w : workers)
		delete w;
}

void coTaskScheduler::Add(coTask& task)
{
	if (task.IsReady())
	{
		lock.Lock();
		coPushBack(readyTasks[task.priority], &task);
		waitCondition.WakeAll();
		lock.Unlock();
	}
	else
	{
		coPushBack(waitingTasks[task.priority], &task);
	}
}

void coTaskScheduler::_ExecuteOneTask(const coTaskContext& context)
{
	waitCondition.Reset();

	lock.Lock();
	coTask* task = nullptr;
	for (coDynamicQueue<coTask*>& tasks : readyTasks)
	{
		if (tasks.count)
		{
			task = coPopFront(tasks);
			break;
		}
	}
	lock.Unlock();
	if (!task)
	{
		waitCondition.Wait();
		return;
	}

	task->Execute(context);
	coTask* nextTask = task->next;
	if (nextTask)
	{
		if (--nextTask->nbActiveDependencies == 0)
		{
			lock.Lock();
			coRemoveUnordered(waitingTasks[nextTask->priority], nextTask);
			coPushBack(readyTasks[nextTask->priority], nextTask);
			lock.Unlock();
		}
	}
}

void coTaskScheduler::OnStop()
{
	for (coTaskWorkerThread* w : workers)
	{
		w->RequestStop();
	}
	waitCondition.WakeAll();
	for (coTaskWorkerThread* w : workers)
	{
		w->Stop();
	}
	Super::OnStop();
}

coResult coTaskScheduler::OnStart()
{
	coTRY(Super::OnStart(), nullptr);

	for (coTaskWorkerThread* w : workers)
	{
		coTRY(w->Start(), nullptr);
	}

	return true;
}
