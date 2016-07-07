// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "pattern/pch.h"
#include "pattern/object/coObject.h"

coObject::coObject()
	: objectState(ObjectState::NONE)
{

}

coResult coObject::Init(const InitConfig& desc)
{
	if (objectState >= ObjectState::INITIALIZED)
		return true;

	OnInit(desc);
	objectState = ObjectState::INITIALIZED;
	return true;
}

coResult coObject::Start()
{
	if (objectState >= ObjectState::STARTED)
		return true;

	OnStart();
	objectState = ObjectState::STARTED;
	return true;
}

void coObject::Stop()
{
	if (objectState < ObjectState::STARTED)
		return;

	OnStop();
	objectState = ObjectState::INITIALIZED;
}

coResult coObject::OnInit(const InitConfig& desc)
{
#ifdef coDEBUG
	debugName = desc.debugName;
#endif
	return true;
}

coResult coObject::OnStart()
{
	return true;
}

void coObject::OnStop()
{
}
