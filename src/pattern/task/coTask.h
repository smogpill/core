// Copyright(c) 2019-2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/scalar/coAtomicInt32.h"
#include <lang/reflect/coTypeDecl.h>
#include <debug/log/coAssert.h>
#include "coTaskSetup.h"
#include "coTaskFunction.h"
class coTaskContext;
class coTaskBarrier;
class coTaskManager;

class coTask
{
public:
	
	virtual ~coTask() {}
	void SetFunction(const coTaskFunction& function) { _function = function; }
	void SetSetup(const coTaskSetup& setup);
	void AddRef();
	void RemoveRef();
	void AddDependency(coUint nb = 1);
	/// Returns true whenever the dependency counter reaches zero
	coBool RemoveDependency(coUint nb = 1);
	/// Job will be queued whenever the dependency counter reaches zero
	void RemoveDependencyAndQueue(coUint nb = 1);

	coBool SetBarrier(coTaskBarrier* barrier);
	coUint32 Execute();
	coBool CanBeExecuted() const { return _nbDependencies.load(std::memory_order_relaxed) == 0; }
	coBool IsDone() const { return _nbDependencies.load(std::memory_order_relaxed) == s_doneState; }
	coTaskPriority GetPriority() const { return _priority; }
	coUint32 GetNbDependencies() const { return _nbDependencies.load(std::memory_order_relaxed); }

	static constexpr coUint32 s_executingState = 0xe0e0e0e0;
	static constexpr coUint32 s_doneState = 0xd0d0d0d0;
	static constexpr coIntPtr s_barrierDoneState = ~coIntPtr(0);

private:
	coTaskManager& GetSystem() const;

	friend class coTaskScheduler;
	coTaskPriority _priority = coTaskPriority::FRAME;
	std::atomic<coIntPtr> _barrier = 0;
	coTaskFunction _function;
	std::atomic<coUint32> _nbRefs = 0;
	std::atomic<coUint32> _nbDependencies = 1;
};

#include "coTask.inl"
