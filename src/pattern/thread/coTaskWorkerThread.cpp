// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/thread/coTaskWorkerThread.h"
#include "pattern/thread/coTaskScheduler.h"
#include "lang/result/coResult_f.h"

coResult coTaskWorkerThread::OnRun()
{
	coTRY(Super::OnRun(), nullptr);

	while (!IsStopRequested())
	{
		scheduler->_ExecuteOneTask();
	}
	return true;
}
