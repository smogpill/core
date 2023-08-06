// Copyright(c) 2019-2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/scalar/coAtomicInt32.h"
#include <lang/reflect/coTypeDecl.h>
#include <debug/log/coAssert.h>
class coTaskContext;
class coTaskBarrier;
class coTaskSystem;

class coTask
{
public:
	enum Priority
	{
		HIGH,
		DEFAULT,
		LOW,

		END
	};
	virtual ~coTask() {}
	void SetPriority(Priority priority_) { priority = priority; }
	void AddRef();
	void RemoveRef();
	void AddDependency(coUint nb);
	/// Returns true whenever the dependency counter reaches zero
	coBool RemoveDependency(coUint nb);
	/// Job will be queued whenever the dependency counter reaches zero
	void RemoveDependencyAndQueue(coUint nb);

	coBool SetBarrier(coTaskBarrier* barrier);
	coUint32 Execute();
	coBool CanBeExecuted() const { return _nbDependencies.load(std::memory_order_relaxed) == 0; }
	coBool IsDone() const { return _nbDependencies.load(std::memory_order_relaxed) == s_doneState; }

	static constexpr coUint32 s_executingState = 0xe0e0e0e0;
	static constexpr coUint32 s_doneState = 0xd0d0d0d0;
	static constexpr coIntPtr s_barrierDoneState = ~coIntPtr(0);

private:
	coTaskSystem& GetSystem() const;

	friend class coTaskScheduler;
	Priority priority = Priority::DEFAULT;
	std::atomic<coIntPtr> _barrier = 0;
	std::function<void()> _function;
	std::atomic<coUint32> _nbRefs = 0;
	std::atomic<coUint32> _nbDependencies = 0;
};

#include "coTask.inl"