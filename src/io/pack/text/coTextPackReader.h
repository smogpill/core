// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"
#include "container/string/coConstString.h"

class coPack;
class coPackFormat;

class coTextPackReader
{
public:
	coResult Read(const coChar* text);

private:
	coResult ReadBlock();
	coResult ReadStatement();
	coResult ReadExpression();
	coResult IgnoreFirstAndReadStringValue();
	coResult IgnoreFirstAndReadTrueValue();
	coResult IgnoreFirstAndReadFalseValue();
	coResult ReadNumberValue();
	coResult ReadNegativeNumberValue();
	coResult ReadIdentifier();
	void ReadWhitespace();

	coPackFormat* format = nullptr;
	coPack* pack = nullptr;
	const coChar* text = nullptr;
	coUint32 curPos = 0;
};
