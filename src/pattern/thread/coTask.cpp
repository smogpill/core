// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/thread/coTask.h"
#include "math/scalar/coAtomicInt32_f.h"

void coTask::SetNext(coTask* task)
{
	if (next)
	{
		--next->nbActiveDependencies;
	}
	next = task;
	if (next)
	{
		++next->nbActiveDependencies;
	}
}
