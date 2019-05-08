// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "pattern/thread/coThread.h"
#include "lang/reflect/coTypeDecl.h"

class coTaskScheduler;

class coTaskWorkerThread : public coThread
{
	coDECLARE_SUPER(coThread);
public:
	coTaskWorkerThread(coTaskScheduler& scheduler_) : scheduler(&scheduler_) {}

protected:
	virtual coResult OnRun() override;

	coTaskScheduler* scheduler = nullptr;
};
