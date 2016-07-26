// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coParsedProject;
class coProjectGeneratorPlugin;

class coProjectGenerator : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coConstString projectDir;
		coConstString outDir;
		coConstString precompiledHeaderRelativePath;
		coArray<coProjectGeneratorPlugin*> plugins;
	};

	coResult Generate(const coParsedProject& _parsedProject);

	const coConstString& GetProjectDir() const { return projectDir; }
	const coConstString& GetOutDir() const { return outDir; }
	const coConstString& GetPrecompiledHeaderRelativepath() const { return precompiledHeaderRelativePath; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

	coDynamicString projectDir;
	coDynamicString outDir;
	coDynamicString precompiledHeaderRelativePath;
	coDynamicArray<coProjectGeneratorPlugin*> plugins;
};
