// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"

class coThread
{
public:
	coThread();
	virtual ~coThread();

	coResult _Run() { return OnRun(); }

protected:
	virtual coResult OnRun() { return true; }

private:


	coUint64 plateformSpecificData = 0;
};
