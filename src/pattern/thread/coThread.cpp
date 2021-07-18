// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/thread/coThread.h"
#include "debug/profiler/coProfile.h"

coThread::~coThread()
{
	Stop();
}

void coThread::SetAffinityMask(coUint32 mask)
{
	coASSERT(!IsStarted());
	affinityMask = mask;
}

coResult coThread::_Run()
{
	return OnRun();
}