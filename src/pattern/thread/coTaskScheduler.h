// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "pattern/object/coObject.h"
#include "pattern/thread/coWaitCondition.h"
#include "pattern/thread/coTaskContext.h"
#include "pattern/thread/coLock.h"
#include "container/array/coDynamicArray.h"
class coTaskWorkerThread;
class coTask;

class coTaskScheduler : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	coTaskScheduler(coUint nbWorkers);
	~coTaskScheduler();

	void Add(coTask& task);
	void _ExecuteOneTask();

protected:
	virtual coResult OnStart() override;
	virtual void OnStop() override;

private:
	coDynamicArray<coTaskWorkerThread*> workers;
	coDynamicArray<coTask*> tasks;
	coWaitCondition waitCondition;
	coTaskContext context;
	coLock lock;
};
