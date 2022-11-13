// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "io/stream/coOutputStream.h"

class coBinaryOutputStream final : public coOutputStream
{
	coDECLARE_BASE(coOutputStream);
public:
};

coFORCE_INLINE coBinaryOutputStream& operator<<(coBinaryOutputStream& stream, coBool v)
{
	stream.Write(v);
	return stream;
}

coFORCE_INLINE coBinaryOutputStream& operator<<(coBinaryOutputStream& stream, coFloat v)
{
	stream.Write(reinterpret_cast<const coByte*>(&v), sizeof(v));
	return stream;
}

coFORCE_INLINE coBinaryOutputStream& operator<<(coBinaryOutputStream& stream, coUint8 v)
{
	stream.Write(v);
	return stream;
}

coFORCE_INLINE coBinaryOutputStream& operator<<(coBinaryOutputStream& stream, coUint32 v)
{
	stream.Write(reinterpret_cast<const coByte*>(&v), sizeof(v));
	return stream;
}

coFORCE_INLINE coBinaryOutputStream& operator<<(coBinaryOutputStream& stream, coUint64 v)
{
	stream.Write(reinterpret_cast<const coByte*>(&v), sizeof(v));
	return stream;
}

template <class T>
coFORCE_INLINE coBinaryOutputStream& operator<<(coBinaryOutputStream& stream, const T& a)
{
	a.Write(stream);
	return stream;
}
