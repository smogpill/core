// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "coTextArchiveReader.h"
#include "../coArchive_f.h"
#include "../coArchiveFormat.h"
#include "lang/result/coResult_f.h"
#include "container/string/coChar_f.h"

coTextArchiveReader::coTextArchiveReader(coArchive& pack_, const coArchiveFormat& format_)
	: pack(pack_)
	, format(format_)
{

}

coResult coTextArchiveReader::Read(const coChar* text_)
{
	curPos = 0;
	text = text_;
	PassWhitespace();
	coTRY(ReadBlock(), nullptr);
	PassWhitespace();
	coTRY(text[curPos] == '\0', nullptr);
	return true;
}

coResult coTextArchiveReader::ReadBlock()
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

coResult coTextArchiveReader::ReadStatement()
{
	coTRY(ReadIdentifier(), nullptr);
	PassWhitespace();
	coTRY(text[curPos++] == '=', nullptr);
	PassWhitespace();
	coTRY(ReadExpression(), nullptr);
	coTRY(text[curPos++] == ';', nullptr);
	return true;
}

coResult coTextArchiveReader::ReadExpression()
{
	const coChar c = text[curPos];
	switch (c)
	{
	case '{':
	{
		coTRY(ReadBlock(), nullptr);
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

coResult coTextArchiveReader::IgnoreFirstAndReadStringValue()
{
	coASSERT(text[curPos] == '"');
	const coUint startPos = ++curPos;
	do
	{
		switch (text[curPos])
		{
		case '"':
		{
			coPushValue(pack, curField, coConstString(&text[curPos], curPos - startPos));
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

coResult coTextArchiveReader::IgnoreFirstAndReadTrueValue()
{
	coASSERT(text[curPos] == 't');
	++curPos;
	coBool ok;
	ok = text[curPos++] == 'r';
	ok &= text[curPos++] == 'u';
	ok &= text[curPos++] == 'e';
	coPushValue(pack, curField, true);
	return ok;
}

coResult coTextArchiveReader::IgnoreFirstAndReadFalseValue()
{
	coASSERT(text[curPos] == 'f');
	++curPos;
	coBool ok;
	ok = text[curPos++] == 'a';
	ok &= text[curPos++] == 'l';
	ok &= text[curPos++] == 's';
	ok &= text[curPos++] == 'e';
	coPushValue(pack, curField, false);
	return ok;
}

void coTextArchiveReader::PassWhitespace()
{
	while (coIsWhitespace(text[curPos]))
		++curPos;
}

coResult coTextArchiveReader::ReadNumberValue()
{
	return true;
}

coResult coTextArchiveReader::IgnoreFirstAndReadNegativeNumberValue()
{
	coASSERT(text[curPos] == '-');
	++curPos;
	return true;
}

coResult coTextArchiveReader::ReadIdentifier()
{
	coUint32 startPos = curPos;
	if (coIsIdentifierHeadCompatible(text[curPos]))
	{
		++curPos;
		while (coIsIdentifierBodyCompatible(text[curPos]))
			++curPos;

		coConstString identifier;
		identifier.data = &text[startPos];
		identifier.count = curPos - startPos;
		curField = format.GetIndex(identifier);
		return curField != coArchiveFormat::s_invalidFieldIndex;
	}
	return false;
}
