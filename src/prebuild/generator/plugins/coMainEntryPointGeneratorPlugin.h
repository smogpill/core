// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "prebuild/generator/coProjectGeneratorPlugin.h"

class coMainEntryPointGeneratorPlugin : public coProjectGeneratorPlugin
{
	coDECLARE_SUPER(coProjectGeneratorPlugin);
public:
	virtual coResult Generate(const coParsedProject& _parsedProject) override;

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coResult GenerateCpp();

	coDynamicString projectGenDir;
};
