// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "pattern/pimpl/coPimpl.h"

class coWaitCondition
{
public:
	coWaitCondition();
	~coWaitCondition();
	void Reset();
	void WakeAll();
	void Wait();
	coBool Wait(coUint milliseconds);

private:
	coDECLARE_PIMPL(8, 8);
};
