// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"
#include "container/string/coConstString.h"

class coSourceParser;
class coParsedType;
class coParsedProject;

class coProjectParser : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
	};
	class ParseConfig
	{
	public:
		ParseConfig();

		coConstString srcReferenceDir;
		coConstString outReferenceDir;
		coConstString projectRelativePath;
		coConstString precompiledHeaderPath;
		coArray<coConstString> includeDirs;
	};

	coProjectParser();
	virtual ~coProjectParser();
	coResult Parse(coParsedProject& _out, const ParseConfig& _config);

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;
private:
	coResult ParseSourceDir(coParsedProject& _out, const ParseConfig& _config, const coConstString& _relativePath);
	coResult ParseSourceFile(coParsedProject& _out, const ParseConfig& _config, const coConstString& _relativePath);

	coDynamicString srcProjectDir;
	coDynamicString outProjectDir;
	coSourceParser* sourceParser;
};
