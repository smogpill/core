// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/stream/coOutputStream.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coNumericLimits.h"

coOutputStream::~coOutputStream()
{
	for (auto* p : blocks)
		delete p;
}

void coOutputStream::Write(coByte value)
{
	if (coUNLIKELY(cursor == windowEnd))
	{
		if (!(*refill)())
			return 0;
	}
	*cursor = value;
	++cursor;
}

void coOutputStream::Write(const void* data, coUint size8)
{
	const coUint desiredSize8 = size8;
	do
	{
		if (coLIKELY(cursor != windowEnd)); // likely
		else
		{
			if (!(*refill)())
				return desiredSize8 - size8;
		}
		coASSERT(windowEnd - cursor < coNumericLimits<coUint>::Max());
		const coUint available8 = static_cast<coUint>(windowEnd - cursor);
		const coUint writeSize8 = (available8 >= size8) ? size8 : available8;
		coMemCopy(cursor, data, writeSize8);
		size8 -= writeSize8;
		data += writeSize8;
		cursor += writeSize8;
	} while (size8);
}
