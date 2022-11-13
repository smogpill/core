// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/result/coResult.h"
#include "lang/reflect/coTypeDecl.h"
#include "container/string/coConstString.h"
#include "container/string/coDynamicString.h"

class coObject
{
	coDECLARE_CLASS(coObject);
public:
	enum class ObjectState : coUint8
	{
		none,
		initialized,
		started
	};

	class InitConfig
	{
	public:
		virtual ~InitConfig() {}
		coConstString debugName;
	};

	coObject();
	virtual ~coObject();

	coResult Init(const InitConfig& _config);
	coResult Init();
	coResult Start();
	void Stop();
	virtual const coConstString& GetDebugName() const;
	void SetDebugName(const coConstString& _s);
	coBool IsInitialized() const { return objectState == ObjectState::initialized; }
	coBool IsStarted() const { return objectState == ObjectState::started; }
	virtual coBool IsValid() const { return true; }

protected:
	virtual coResult OnInit();
	virtual coResult OnInit(const InitConfig& _config);
	virtual coResult OnStart();
	virtual void OnStop();

private:
	ObjectState objectState;
#ifdef coDEBUG
	coDynamicString debugName;
#endif
};

coDynamicString& operator<<(coDynamicString& _s, const coObject& _o);
