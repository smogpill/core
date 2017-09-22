// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/object/coObject.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coType.h"

coObject::coObject()
	: objectState(ObjectState::none)
{

}

coObject::~coObject()
{
	coASSERT(!IsStarted());
}

coResult coObject::Init(const InitConfig& _config)
{
	if (objectState >= ObjectState::initialized)
		return true;

	coTRY(OnInit(_config), "Init failed");
	objectState = ObjectState::initialized;
	return true;
}

coResult coObject::Init()
{
	if (objectState >= ObjectState::initialized)
		return true;

	coTRY(OnInit(), "Init failed");
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

coResult coObject::OnInit()
{
	return true;
}

coResult coObject::OnInit(const InitConfig& _config)
{
	(void)_config;
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
	(void)_s;
#ifdef coDEBUG
	debugName = _s;
#endif
}

coDynamicString& operator<<(coDynamicString& _s, const coObject& _o)
{
	const coConstString& debugName = _o.GetDebugName();
	if (debugName != "")
	{
		_s << debugName;
	}
	else
	{
		const coType* type = _o.GetType();
		if (type)
		{
			_s << "<" << type->name << ">";
		}
		else
		{
			_s << "<unknown>";
		}
	}
	return _s;
}
