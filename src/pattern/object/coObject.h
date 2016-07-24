// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/result/coResult.h"
#include "lang/reflect/coTypeDecl.h"
#include "container/string/coConstString.h"
#include "container/string/coDynamicString.h"

class coObject
{
	//coDECLARE_ROOT_TYPE(coObject);
	coDECLARE_TYPE(coObject);
	coDECLARE_REFLECTED(coObject);
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

	coResult Init(const InitConfig& _config);
	coResult Start();
	void Stop();
	const coConstString& GetDebugName() const;
	void SetDebugName(const coConstString& _s);

protected:
	virtual coResult OnInit(const InitConfig& _config);
	virtual coResult OnStart();
	virtual void OnStop();

private:
	ObjectState objectState;
#ifdef coDEBUG
	coDynamicString debugName;
#endif
};
