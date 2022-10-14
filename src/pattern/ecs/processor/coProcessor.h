// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/result/coResult.h>
class coEntity;
class coComponent;

class coProcessorInfo
{
public:

};

class coProcessor
{
protected:
	virtual void OnInit(coComponent& component) {}
	virtual void OnStart(coComponent& component) {}
	virtual void OnStop(coComponent& component) {}
	virtual void OnShutdown(coComponent& component) {}
};

#define coDECLARE_PROCESSOR(_self_, _parent_)\
	private:\
	coDECLARE_BASE(_parent_)