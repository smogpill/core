// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"

class coArchive;
class coArchiveFormat;

/*
class coTextArchiveReader
{
public:
	coTextArchiveReader(coArchive& pack, const coArchiveFormat& format);
	coResult Read(const coChar* text);

private:
	coResult ReadBlock();
	coResult ReadStatement();
	coResult ReadExpression();
	coResult IgnoreFirstAndReadStringValue();
	coResult IgnoreFirstAndReadTrueValue();
	coResult IgnoreFirstAndReadFalseValue();
	coResult ReadNumberValue();
	coResult IgnoreFirstAndReadNegativeNumberValue();
	coResult ReadIdentifier();
	void PassWhitespace();

	const coArchiveFormat& format;
	coArchive& pack;
	const coChar* text = nullptr;
	coUint32 curPos = 0;
	coUint8 curField = 0;
};
*/