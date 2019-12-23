// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"

class coPack;
class coPackFormat;
class coDynamicString;

class coTextPackWriter
{
public:
	coTextPackWriter(const coPack& pack, const coPackFormat& format);
	coResult Write(coDynamicString& text);

private:
	coResult WriteBlock();
	coResult WriteStatement();
	coResult WriteExpression();
	coResult WriteStringValue();
	coResult WriteTrueValue();
	coResult WriteFalseValue();
	coResult WriteNumberValue();
	coResult WriteIdentifier();

	const coPackFormat& format;
	const coPack& pack;
	coDynamicString* text = nullptr;
};
