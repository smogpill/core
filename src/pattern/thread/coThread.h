// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"
#include "pattern/pimpl/coPimpl.h"

class coThread
{
public:
	coThread();
	virtual ~coThread();

	coResult Start();
	void Stop();

	coResult _Run() { return OnRun(); }

protected:
	virtual coResult OnRun() { return true; }

private:
	coDECLARE_PIMPL(8, 8);
};
