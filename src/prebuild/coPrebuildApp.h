// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "app/coApp.h"
#include "lang/result/coResult.h"
#include "container/string/coDynamicString.h"
#include "parser/project/coProjectParser.h"

class coPrebuildApp : public coApp
{
	typedef coApp Super;
public:
	class InitConfig : public Super::InitConfig
	{
	public:
	};
	coPrebuildApp();
	virtual ~coPrebuildApp();

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;
	virtual coResult OnStart() override;

private:
	coResult ParseArgs(const InitConfig& _config);
	coResult ParseProject(coParsedProject& _parsedProject);
	coResult GenerateProject(const coParsedProject& _parsedProject);

	coDynamicString srcReferenceDir;
	coDynamicString outReferenceDir;
	coDynamicString projectRelativePath;
	coDynamicString pchRelativePath;
	coDynamicArray<coDynamicString*> includeDirs;
	coProjectParser projectParser;
};
