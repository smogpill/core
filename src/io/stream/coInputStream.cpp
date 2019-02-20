// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/stream/coInputStream.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coNumericLimits.h"

void coInputStream::Read(coByte& _v)
{
	if (coLIKELY(pos < buffer.count))
	{
		_v = buffer[pos++];
	}
	else
	{
		SetErrorMode();
	}
}

void coInputStream::Read(coByte* _data, coUint _size8)
{
	const coUint32 newPos = pos + _size8;
	if (coLIKELY(newPos <= buffer.count))
	{
		coMemCopy(_data, &buffer[pos], _size8);
		pos = newPos;
	}
	else
	{
		SetErrorMode();
	}
}
