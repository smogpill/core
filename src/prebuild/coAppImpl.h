// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "app/coApp.h"
#include "lang/result/coResult.h"
#include "container/string/coDynamicString.h"

class coReflectParser;

class coAppImpl : public coApp
{
	typedef coApp Super;
public:
	class InitConfig : public Super::InitConfig
	{
	public:
	};
	coAppImpl();
	virtual ~coAppImpl();

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;
	virtual coResult OnStart() override;

private:
	coResult ParseArgs(const InitConfig& _config);

	coDynamicString projectDir;
	coReflectParser* parser;
};
