// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/result/coResult.h>
class coEntity;

class coProcessorInfo
{
public:

};

class coProcessor
{
protected:
	virtual void OnInit(coProcessorInfo& info) {}
	virtual void OnStart(coProcessorInfo& info) {}
	virtual void OnStop(coProcessorInfo& info) {}
	virtual void OnShutdown(coProcessorInfo& info) {}
};

#define coDECLARE_PROCESSOR(_self_, _parent_)\
	private:\
	coDECLARE_BASE(_parent_)