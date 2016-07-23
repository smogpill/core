// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "parser/coParser.h"
#include "container/array/coDynamicArray.h"

class coParsedType;

class coReflectParser : public coParser
{
public:
	class ParseConfig
	{
	public:
		coConstString filePath;
	};

	const coArray<coParsedType*>& getTypes() const { return parsedTypes; }
	virtual coResult Parse(const ParseConfig& _config);
	static coReflectParser* Create();

protected:
	coReflectParser(){}
	coDynamicArray<coParsedType*> parsedTypes;
};
