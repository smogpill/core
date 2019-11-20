// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "coTextPackReader.h"
#include "../coPack_f.h"
#include "lang/result/coResult_f.h"
#include "container/string/coChar_f.h"

coTextPackReader::coTextPackReader(coPack& pack_)
	: pack(pack_)
{

}

coResult coTextPackReader::Read(const coChar* text_)
{
	curPos = 0;
	text = text_;
	PassWhitespace();
	coTRY(ReadBlock(), nullptr);
	PassWhitespace();
	coTRY(text[curPos] == '\0', nullptr);
	return true;
}

coResult coTextPackReader::ReadBlock()
{
	coTRY(text[curPos++] == '{', nullptr);
	do
	{
		PassWhitespace();
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
	coConstString identifier;
	coTRY(ReadIdentifier(identifier), nullptr);
	coTRY(text[curPos++] == ':', nullptr);
	PassWhitespace();
	coTRY(ReadExpression(), nullptr);
	coTRY(text[curPos++] == ';', nullptr);
	return true;
}

coResult coTextPackReader::ReadExpression()
{
	const coChar c = text[curPos];
	switch (c)
	{
	case '{':
	{
		break;
	}
	case '-':
	{
		coTRY(IgnoreFirstAndReadNegativeNumberValue(), nullptr);
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
	coASSERT(text[curPos] == '"');
	const coUint startPos = ++curPos;
	do
	{
		switch (text[curPos])
		{
		case '"':
		{
			coPushBack(pack, coConstString(&text[curPos], curPos - startPos));
			++curPos;
			return true;
		}
		case '\0':
		{
			coERROR("Early end.");
			return false;
		}
		}
		++curPos;
	} while (true);
}

coResult coTextPackReader::IgnoreFirstAndReadTrueValue()
{
	coASSERT(text[curPos] == 't');
	++curPos;
	coBool ok;
	ok = text[curPos++] == 'r';
	ok &= text[curPos++] == 'u';
	ok &= text[curPos++] == 'e';
	coPushBack(pack, true);
	return ok;
}

coResult coTextPackReader::IgnoreFirstAndReadFalseValue()
{
	coASSERT(text[curPos] == 'f');
	++curPos;
	coBool ok;
	ok = text[curPos++] == 'a';
	ok &= text[curPos++] == 'l';
	ok &= text[curPos++] == 's';
	ok &= text[curPos++] == 'e';
	coPushBack(pack, false);
	return ok;
}

void coTextPackReader::PassWhitespace()
{
	while (coIsWhitespace(text[curPos]))
		++curPos;
}

coResult coTextPackReader::ReadNumberValue()
{
	return true;
}

coResult coTextPackReader::IgnoreFirstAndReadNegativeNumberValue()
{
	coASSERT(text[curPos] == '-');
	++curPos;
	return true;
}

coResult coTextPackReader::ReadIdentifier(coConstString& identifier)
{
	coUint32 startPos = curPos;
	if (coIsIdentifierHeadCompatible(text[curPos]))
	{
		++curPos;
		while (coIsIdentifierBodyCompatible(text[curPos]))
			++curPos;

		identifier.data = &text[startPos];
		identifier.count = curPos - startPos;
		return true;
	}
	return false;
}
