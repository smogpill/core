// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "io/stream/coInputStream.h"

class coBinaryInputStream final : public coInputStream
{
	coDECLARE_BASE(coInputStream);
public:
	coBinaryInputStream(const coArray<coByte>& buffer);
};

coFORCE_INLINE coBinaryInputStream& operator >> (coBinaryInputStream& stream, coBool& v)
{
	stream.Read(reinterpret_cast<coByte&>(v));
	return stream;
}

coFORCE_INLINE coBinaryInputStream& operator >> (coBinaryInputStream& stream, coUint8& v)
{
	stream.Read(v);
	return stream;
}

coFORCE_INLINE coBinaryInputStream& operator >> (coBinaryInputStream& stream, coUint16& v)
{
	stream.Read(reinterpret_cast<coByte*>(&v), sizeof(v));
	return stream;
}

coFORCE_INLINE coBinaryInputStream& operator >> (coBinaryInputStream& stream, coUint32& v)
{
	stream.Read(reinterpret_cast<coByte*>(&v), sizeof(v));
	return stream;
}

coFORCE_INLINE coBinaryInputStream& operator >> (coBinaryInputStream& stream, coUint64& v)
{
	stream.Read(reinterpret_cast<coByte*>(&v), sizeof(v));
	return stream;
}

coFORCE_INLINE coBinaryInputStream& operator >> (coBinaryInputStream& stream, coFloat& v)
{
	stream.Read(reinterpret_cast<coByte*>(&v), sizeof(v));
	return stream;
}

template <class T>
coFORCE_INLINE coBinaryInputStream& operator>>(coBinaryInputStream& stream, T& a)
{
	a.Read(stream);
	return stream;
}

template <class T>
coBinaryInputStream& operator>>(coBinaryInputStream& stream, coDynamicArray<T>& a)
{
	coUint32 nb;
	stream >> nb;
	coClear(a);
	if (nb)
	{
		coResize(a, nb);
		for (T& v : a)
			stream >> v;
	}
	return stream;
}
