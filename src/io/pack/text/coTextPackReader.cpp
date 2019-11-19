// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "coTextPackReader.h"
#include "lang/result/coResult_f.h"
#include "container/string/coChar_f.h"

coResult coTextPackReader::Read(const coConstString& text_)
{
	text = text_;
	ReadWhitespace();
	coTRY(ReadBlock(), nullptr);
	return true;
}

coResult coTextPackReader::ReadBlock()
{
	coTRY(text[curPos++] == '{', nullptr);
	ReadWhitespace();
	
	ReadWhitespace();
	coTRY(text[curPos++] == '}', nullptr);
	return true;
}

coResult coTextPackReader::ReadStatement()
{
	coTRY(ReadIdentifier(), nullptr);
	coTRY(text[curPos++] == ':', nullptr);
	ReadWhitespace();
	coTRY(ReadExpression(), nullptr);
	return true;
}

coResult coTextPackReader::ReadExpression()
{
	return true;
}

void coTextPackReader::ReadWhitespace()
{
	while (coIsWhitespace(text[curPos]))
		++curPos;
}

coResult coTextPackReader::ReadIdentifier()
{
	coUint32 startPos = curPos;
	if (coIsIdentifierHeadCompatible(text[curPos]))
	{
		++curPos;
		while (coIsIdentifierBodyCompatible(text[curPos]))
			++curPos;
	}	
	return true;
}
