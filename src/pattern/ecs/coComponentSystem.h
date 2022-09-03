// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../singleton/coSingleton.h"
#include <container/pool/coPool_f.h>
#include <lang/result/coResult.h>

class coEntityHandle;

class coComponentSystem
{
public:
protected:
	virtual coResult OnInit(const coEntityHandle& entityH) {}
	virtual coResult OnStart(const coEntityHandle& entityH) {}
	virtual void OnStop(const coEntityHandle& entityH) {}
	virtual void OnShutdown(const coEntityHandle& entityH) {}
	virtual void OnWrite(const coEntityHandle& entityH, coBinaryOutputStream& stream) const {}
	virtual void OnRead(const coEntityHandle& entityH, coBinaryInputStream& stream) {}

private:
};
