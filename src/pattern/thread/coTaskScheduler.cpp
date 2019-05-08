// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/thread/coTaskScheduler.h"
#include "pattern/thread/coTaskWorkerThread.h"
#include "pattern/thread/coTaskContext.h"

coTaskScheduler::coTaskScheduler(coUint nbWorkers)
{
	for (coUint i = 0; i < nbWorkers; ++i)
	{
		coPushBack(workers, new coTaskWorkerThread(*this));
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
	coPushBack(tasks, &task);
}

void coTaskScheduler::_ExecuteOneTask()
{
	waitCondition.Wait();

}

void coTaskScheduler::OnStop()
{
	for (coTaskWorkerThread* w : workers)
	{
		w->Stop();
	}
	Super::OnStop();
}

coResult coTaskScheduler::OnStart()
{
	Super::OnStart();
	coTaskContext context;
	context.scheduler = this;

	for (coTaskWorkerThread* w : workers)
	{
		w->Start();
	}

	return true;
}
