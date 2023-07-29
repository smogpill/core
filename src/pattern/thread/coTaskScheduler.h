// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coTask.h"
#include "pattern/object/coObject.h"
#include "pattern/thread/coWaitCondition.h"
#include "pattern/thread/coTaskContext.h"
#include "pattern/lock/coMutex.h"
#include "pattern/singleton/coSingleton.h"
#include "container/array/coDynamicArray.h"
#include "container/queue/coDynamicQueue.h"
class coTaskWorkerThread;

class coTaskScheduler : public coObject
{
	coDECLARE_SINGLETON(coTaskScheduler);
	coDECLARE_BASE(coObject);
public:
	coTaskScheduler(coUint nbWorkers = coUint(-1));
	~coTaskScheduler();

	void Add(coTask& task);
	void _ExecuteOneTask(const coTaskContext& context);

protected:
	virtual coResult OnStart() override;
	virtual void OnStop() override;

private:
	coDynamicArray<coTaskWorkerThread*> workers;
	coDynamicQueue<coTask*> readyTasks[coTask::Priority::END];
	coDynamicArray<coTask*> waitingTasks[coTask::Priority::END];
	coWaitCondition waitCondition;
	mutable coMutex _mutex;
};
