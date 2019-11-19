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
	coResult Read(const coConstString& text);

private:
	coResult ReadBlock();
	coResult ReadStatement();
	coResult ReadExpression();
	coResult ReadIdentifier();
	void ReadWhitespace();

	coPackFormat* format = nullptr;
	coPack* pack = nullptr;
	coConstString text;
	coUint32 curPos = 0;
};
