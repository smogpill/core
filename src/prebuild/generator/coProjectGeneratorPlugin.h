// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coParsedProject;
class coProjectGenerator;

class coProjectGeneratorPlugin : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
	};

	coProjectGeneratorPlugin();

	virtual coResult Generate(const coParsedProject& _parsedProject);
	void _SetProjectGenerator(coProjectGenerator& _projectGenerator);

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

	coProjectGenerator* projectGenerator;
};
