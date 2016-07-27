// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "prebuild/generator/coProjectGeneratorPlugin.h"

class coParsedProject;
class coParsedType;
class coDynamicString;

class coCppReflectGeneratorPlugin : public coProjectGeneratorPlugin
{
	coDECLARE_SUPER(coProjectGeneratorPlugin);
public:
	virtual coResult Generate(const coParsedProject& _parsedProject);

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coResult GenerateTypes(const coParsedProject& _parsedProject);
	coResult GenerateType(coDynamicString& _outPath, const coParsedType& _parsedType);

	coDynamicString genDir;
};
