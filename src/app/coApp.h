// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coDefaultLogHandler;

class coApp : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	coApp();
	virtual ~coApp();

	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		const coChar** argv;
		coUint nbArgs;
	};

	coResult ProcessEvents();

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;
	coBool exitRequested;

private:
	coDefaultLogHandler* defaultLogHandler;
};
