// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/scalar/coAtomicInt32.h"
class coTaskContext;

class coTask
{
public:
	virtual ~coTask() {}
	virtual void Execute(const coTaskContext& context) {}
	void SetNext(coTask* task);
	coBool IsReady() const { return nbActiveDependencies == 0; }

	coTask* next = nullptr;
	coAtomicInt32 nbActiveDependencies = 0;
};
