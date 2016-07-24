// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "parser/coParser.h"
#include "container/array/coDynamicArray.h"
#include "lang/reflect/coTypeDecl.h"

class coParsedType;

class coReflectParser : public coParser
{
	coDECLARE_DERIVED(coParser);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
	};
	class ParseConfig
	{
	public:
		ParseConfig();
		coConstString filePath;
		coConstString outPath;
		coBool precompiledHeader;
	};

	const coArray<coParsedType*>& getTypes() const { return parsedTypes; }
	virtual coResult ParsePrecompiledHeader(const ParseConfig& _config);
	virtual coResult Parse(const ParseConfig& _config);
	static coReflectParser* Create();

protected:
	coReflectParser(){}
	coDynamicArray<coParsedType*> parsedTypes;
};
