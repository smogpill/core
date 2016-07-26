// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/object/coObject.h"
#include "lang/result/coResult_f.h"

coObject::coObject()
	: objectState(ObjectState::none)
{

}

coResult coObject::Init(const InitConfig& _config)
{
	if (objectState >= ObjectState::initialized)
		return true;

	coTRY(OnInit(_config), "Init failed");
	objectState = ObjectState::initialized;
	return true;
}

coResult coObject::Start()
{
	if (objectState >= ObjectState::started)
		return true;

	coTRY(OnStart(), "Start failed");
	objectState = ObjectState::started;
	return true;
}

void coObject::Stop()
{
	if (objectState < ObjectState::started)
		return;

	OnStop();
	objectState = ObjectState::initialized;
}

coResult coObject::OnInit(const InitConfig& _config)
{
#ifdef coDEBUG
	debugName = _config.debugName;
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

const coConstString& coObject::GetDebugName() const
{
#ifdef coDEBUG
	return debugName;
#else
	return coConstString::GetEmpty();
#endif
}

void coObject::SetDebugName(const coConstString& _s)
{
#ifdef coDEBUG
	debugName = _s;
#endif
}
