// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/thread/coTaskWorkerThread.h"
#include "pattern/thread/coTaskScheduler.h"
#include "lang/result/coResult_f.h"
#include "debug/profiler/coProfile.h"

coTaskWorkerThread::coTaskWorkerThread(coTaskScheduler& scheduler_, coUint index_)
	: scheduler(&scheduler_)
	, index(index_)
{
}

coResult coTaskWorkerThread::OnRun()
{
	coTRY(Super::OnRun(), nullptr);
	coPROFILE_THREAD("Worker");

	coTaskContext context;
	context.scheduler = scheduler;
	context.workerIndex = index;
	while (!IsStopRequested())
	{
		scheduler->_ExecuteOneTask(context);
	}
	return true;
}
