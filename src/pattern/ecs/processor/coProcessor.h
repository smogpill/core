// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/result/coResult.h>
class coEntity;

class coProcessor
{
protected:
	virtual coResult OnInit(coEntity& entity) { return true; }
	virtual coResult OnStart(coEntity& entity) { return true; }
	virtual void OnStop(coEntity& entity) {}
	virtual void OnShutdown(coEntity& entity) {}
};
