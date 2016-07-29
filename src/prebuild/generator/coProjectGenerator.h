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
		coConstString outReferenceDir;
		coConstString srcReferenceDir;
		coConstString projectRelativePath;
		coConstString precompiledHeaderRelativePath;
		coArray<coProjectGeneratorPlugin*> plugins;
	};

	coResult Generate(const coParsedProject& _parsedProject);

	const coConstString& GetPrecompiledHeaderRelativePath() const { return precompiledHeaderRelativePath; }
	const coArray<coDynamicString*> GetGeneratedEntryPaths() const { return generatedEntryPaths; }
	const coConstString& GetOutReferenceDir() const { return outReferenceDir; }
	const coConstString& GetSrcReferenceDir() const { return srcReferenceDir; }
	const coConstString& GetProjectRelativePath() const { return projectRelativePath; }
	void AddGeneratedEntryPath(const coConstString& _path);

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

	coDynamicString outReferenceDir;
	coDynamicString srcReferenceDir;
	coDynamicString projectRelativePath;
	coDynamicString precompiledHeaderRelativePath;
	coDynamicArray<coProjectGeneratorPlugin*> plugins;
	coDynamicArray<coDynamicString*> generatedEntryPaths;
};
