// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include <io/stream/coStream.h>

class coInputStream : public coStream
{
	coDECLARE_SUPER(coStream);
public:
	void Read(coByte& value);
	void Read(void* data, coUint size);

	const coArray<coByte> buffer;
};

inline void coInputStream::Read(coByte& value)
{
	if (coLIKELY(pos < buffer.count))
	{
		value = buffer[pos++];
	}
	else
	{
		SetErrorMode();
	}
}

inline void coInputStream::Read(void* data, coUint size)
{
	const coUint32 newPos = pos + size;
	if (coLIKELY(newPos <= buffer.count))
	{
		coMemCopy(data, &buffer[pos], size);
		pos = newPos;
	}
	else
	{
		SetErrorMode();
	}
}
