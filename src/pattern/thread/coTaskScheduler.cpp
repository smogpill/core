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
		coPushBack(readyTasks, &task);
		waitCondition.WakeAll();
		lock.Unlock();
	}
	else
	{
		coPushBack(waitingTasks, &task);
	}
}

void coTaskScheduler::_ExecuteOneTask(const coTaskContext& context)
{
	waitCondition.Reset();

	lock.Lock();
	coTask* task = readyTasks.count ? coPopBack(readyTasks) : nullptr;
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
			coRemoveUnordered(waitingTasks, nextTask);
			coPushBack(readyTasks, nextTask);
			lock.Unlock();
		}
	}
}

void coTaskScheduler::WaitIdle()
{
	while (!IsIdle())
	{
		coSleep(1);
	}
}

coBool coTaskScheduler::IsIdle() const
{
	coBool b;
	lock.Lock();
	b = readyTasks.count == 0 && waitingTasks.count == 0;
	lock.Unlock();
	return b;
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
