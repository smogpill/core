// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "parser/coParser.h"
#include "container/array/coDynamicArray.h"
#include "lang/reflect/coTypeDecl.h"

class coParsedType;

class coSourceParser : public coParser
{
	coDECLARE_SUPER(coParser);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		coConstString precompiledHeaderSourcePath;
		coArray<coConstString> includeDirs;
		coConstString buildDir;
	};
	class ParseConfig
	{
	public:
		ParseConfig();
		coConstString filePath;
	};
	class ParseResult
	{
	public:
		ParseResult();
		coDynamicArray<coParsedType*>* parsedTypes;
	};

	virtual coResult ParsePrecompiledHeader(const ParseConfig& _config);
	virtual coResult Parse(ParseResult& _result, const ParseConfig& _config);
	static coSourceParser* Create();

protected:
	coSourceParser(){}
};
