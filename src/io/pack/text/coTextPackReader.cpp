// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "coTextPackReader.h"
#include "lang/result/coResult_f.h"
#include "container/string/coChar_f.h"

coResult coTextPackReader::Read(const coChar* text_)
{
	curPos = 0;
	text = text_;
	ReadWhitespace();
	coTRY(ReadBlock(), nullptr);
	ReadWhitespace();
	coTRY(text[curPos] == '\0', nullptr);
	return true;
}

coResult coTextPackReader::ReadBlock()
{
	coTRY(text[curPos++] == '{', nullptr);
	do
	{
		ReadWhitespace();
		const coChar current = text[curPos];
		switch (current)
		{
		case '}':
		{
			++curPos;
			return true;
		}
		case '\0':
		{
			coERROR("Early end.");
			return false;
		}
		default:
		{
			coTRY(ReadStatement(), nullptr);
			break;
		}
		}
	} while (true);
}

coResult coTextPackReader::ReadStatement()
{
	coTRY(ReadIdentifier(), nullptr);
	coTRY(text[curPos++] == ':', nullptr);
	ReadWhitespace();
	coTRY(ReadExpression(), nullptr);
	coTRY(text[curPos++] == ';', nullptr);
	return true;
}

coResult coTextPackReader::ReadExpression()
{
	const coUint32 nextPos = curPos + 1;
	const coChar c = text[nextPos];
	switch (c)
	{
	case '{':
	{
		break;
	}
	case '-':
	{
		coTRY(ReadNegativeNumberValue(), nullptr);
		break;
	}
	case '"':
	{
		coTRY(IgnoreFirstAndReadStringValue(), nullptr);
		break;
	}
	case 't':
	{
		coTRY(IgnoreFirstAndReadTrueValue(), nullptr);
		break;
	}
	case 'f':
	{
		coTRY(IgnoreFirstAndReadFalseValue(), nullptr);
		break;
	}
	case '\0':
	{
		coERROR("Early end.");
		return false;
	}
	default:
	{
		break;
	}
	}
	return true;
}

coResult coTextPackReader::IgnoreFirstAndReadStringValue()
{
	return true;
}

coResult coTextPackReader::IgnoreFirstAndReadTrueValue()
{
	coASSERT(text[curPos] == 't');
	++curPos;
	coTRY(text[curPos++] == 'r', nullptr);
	coTRY(text[curPos++] == 'u', nullptr);
	coTRY(text[curPos++] == 'e', nullptr);
	return true;
}

coResult coTextPackReader::IgnoreFirstAndReadFalseValue()
{
	coASSERT(text[curPos] == 'f');
	++curPos;
	coTRY(text[curPos++] == 'a', nullptr);
	coTRY(text[curPos++] == 'l', nullptr);
	coTRY(text[curPos++] == 's', nullptr);
	coTRY(text[curPos++] == 'e', nullptr);
	return true;
}

void coTextPackReader::ReadWhitespace()
{
	while (coIsWhitespace(text[curPos]))
		++curPos;
}

coResult coTextPackReader::ReadNumberValue()
{
	return true;
}

coResult coTextPackReader::ReadNegativeNumberValue()
{
	return true;
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
