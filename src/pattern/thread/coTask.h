// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/scalar/coAtomicInt32.h"
#include <lang/reflect/coTypeDecl.h>
class coTaskContext;

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
	virtual void Execute(const coTaskContext& context) {}
	void SetNext(coTask* task);
	coBool IsReady() const { return nbActiveDependencies == 0; }

protected:
	friend class coTaskScheduler;

	coTask* next = nullptr;
	coAtomicInt32 nbActiveDependencies = 0;

private:
	friend class coTaskScheduler;
	Priority priority = Priority::DEFAULT;
};
