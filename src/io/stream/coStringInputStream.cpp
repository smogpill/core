// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/stream/coStringInputStream.h"
#include "coStringInputStream.h"
#include "container/string/coChar_f.h"

coStringInputStream::coStringInputStream(const coArray<coByte>& buffer)
	: Super(buffer)
{

}

coBool coStringInputStream::PassHorizontalWhitespace()
{
	const coUint32 originPos = pos;
	while (pos < buffer.count)
	{
		if (!coIsHorizontalWhitespace(buffer.data[pos]))
			break;
		++pos;
	}
	return pos != originPos;
}

coBool coStringInputStream::PassVerticalWhitespace()
{
	const coUint32 originPos = pos;
	while (pos < buffer.count)
	{
		if (!coIsVerticalWhitespace(buffer.data[pos]))
			break;
		++pos;
	}
	return pos != originPos;
}

coBool coStringInputStream::PassWhitespaceAndCountLines(coUint32& lines)
{
	lines = 0;
	const coUint32 originPos = pos;
	do
	{
		PassWhitespace();
		if (!coIsVerticalWhitespace(buffer.data[pos]))
			break;
		PassVerticalWhitespace();
		++lines;
	} while (true);
	return pos != originPos;
}

coBool coStringInputStream::PassWhitespace()
{
	const coUint32 originPos = pos;
	while (pos < buffer.count)
	{
		if (!coIsWhitespace(buffer.data[pos]))
			break;
		++pos;
	}
	return pos != originPos;
}

coBool coStringInputStream::PassLine()
{
	const coUint32 originPos = pos;
	while (pos < buffer.count)
	{
		if (coIsVerticalWhitespace(buffer.data[pos]))
			break;
		++pos;
	}
	PassVerticalWhitespace();
	return pos != originPos;
}

coBool coStringInputStream::IsEndOfLine() const
{
	coASSERT(pos < buffer.count);
	return pos == buffer.count || coIsVerticalWhitespace(buffer.data[pos]);
}

coBool coStringInputStream::ReadIdentifier(coConstString& value)
{
	if (!coIsIdentifierHeadCompatible(GetChar()))
		return false;
	coConstString identifier;
	coUint32 offset = 1;
	while (coIsIdentifierBodyCompatible(GetChar(offset)))
		++offset;
	value.data = reinterpret_cast<const coChar*>(&buffer.data[pos]);
	value.count = offset;
	pos += offset;
	return true;
}

coBool coStringInputStream::ReadString(coConstString& value, coChar delimiter)
{
	if (GetChar() != delimiter)
		return false;
	++pos;
	coUint32 offset = 0;
	do
	{
		const coChar c = GetChar(offset);
		if (c == delimiter)
		{
			value.data = reinterpret_cast<const coChar*>(&buffer[pos]);
			value.count = offset++;
			break;
		}
		else if (c == '\0')
		{
			return false;
		}
		else
		{
			++offset;
		}
	} while (true);

	pos += offset;
	return true;
}

coStringInputStream& operator >> (coStringInputStream& stream, coUint32& v)
{
	coChar s[32];
	coUint subPos = 0;
	while (stream.pos < stream.buffer.count)
	{
		const coByte c = stream.buffer.data[stream.pos];
		if (!coIsDigit(c))
			break;
		if (subPos < 31)
			s[subPos++] = c;
		++stream.pos;
	}
	s[subPos] = '\0';
	v = coUint32(atoi(s));
	return stream;
}


coStringInputStream& operator >> (coStringInputStream& stream, coFloat& v)
{
	coChar s[32];
	coUint subPos = 0;
	while (stream.pos < stream.buffer.count)
	{
		const coByte c = stream.buffer.data[stream.pos];
		if (!coIsDigitOrPeriodOrSign(c))
			break;
		if (subPos < 31)
			s[subPos++] = c;
		++stream.pos;
	}
	s[subPos] = '\0';
	v = coFloat(atof(s));
	return stream;
}

/*
coStringInputStream& operator >> (coStringInputStream& stream, coDynamicString& v)
{
	coClear(v);
	coUint32 count = 0;
	stream >> count;
	if (count)
	{

	}
}
*/