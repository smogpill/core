// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/stream/coOutputStream.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coNumericLimits.h"

void coOutputStream::Write(coByte _v)
{
	if (coUNLIKELY(cursor == windowEnd))
	{
		if (!(*refill)())
			return 0;
	}
	*cursor = _v;
	++cursor;
}

void coOutputStream::Write(const coByte* _data, coUint _size8)
{
	const coUint desiredSize8 = _size8;
	do
	{
		if (coLIKELY(cursor != windowEnd)); // likely
		else
		{
			if (!(*refill)())
				return desiredSize8 - _size8;
		}
		coASSERT(windowEnd - cursor < coNumericLimits<coUint>::Max());
		const coUint available8 = static_cast<coUint>(windowEnd - cursor);
		const coUint writeSize8 = (available8 >= _size8) ? _size8 : available8;
		coMemCopy(cursor, _data, writeSize8);
		_size8 -= writeSize8;
		_data += writeSize8;
		cursor += writeSize8;
	} while (_size8);
}
