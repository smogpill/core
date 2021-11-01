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

void coStringInputStream::PassHorizontalWhitespace()
{
	while (pos < buffer.count)
	{
		if (!coIsHorizontalWhitespace(buffer.data[pos]))
			break;
		++pos;
	}
}

void coStringInputStream::PassWhitespace()
{
	while (pos < buffer.count)
	{
		if (!coIsWhitespace(buffer.data[pos]))
			break;
		++pos;
	}
}

void coStringInputStream::PassLine()
{
	while (pos < buffer.count)
	{
		if (coIsVerticalWhitespace(buffer.data[pos]))
			break;
		++pos;
	}
	while (pos < buffer.count)
	{
		if (!coIsVerticalWhitespace(buffer.data[pos]))
			break;
		++pos;
	}
}

coBool coStringInputStream::IsEndOfLine() const
{
	coASSERT(pos < buffer.count);
	return pos == buffer.count || coIsVerticalWhitespace(buffer.data[pos]);
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
