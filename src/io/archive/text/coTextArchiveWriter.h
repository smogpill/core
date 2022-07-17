// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"

class coArchive;
class coArchiveFormat;
class coDynamicString;

class coTextArchiveWriter
{
public:
	coTextArchiveWriter(const coArchive& pack, const coArchiveFormat& format);
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

	const coArchiveFormat& format;
	const coArchive& pack;
	coDynamicString* text = nullptr;
};
