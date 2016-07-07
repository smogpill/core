// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/result/coResult.h"
#include "container/string/coConstString.h"
#include "container/string/coDynamicString.h"

class coObject
{
public:
	enum class ObjectState : coUint8
	{
		NONE,
		INITIALIZED,
		STARTED
	};

	class InitConfig
	{
	public:
		virtual ~InitConfig() {}
		coConstString debugName;
	};

	coObject();
	virtual ~coObject() {}

	coResult Init(const InitConfig& desc);
	coResult Start();
	void Stop();

protected:
	virtual coResult OnInit(const InitConfig& desc);
	virtual coResult OnStart();
	virtual void OnStop();
private:
	ObjectState objectState;
#ifdef coDEBUG
	coDynamicString debugName;
#endif
};
