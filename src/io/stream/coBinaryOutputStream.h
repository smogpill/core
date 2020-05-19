// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "io/stream/coOutputStream.h"

class coBinaryOutputStream final : public coOutputStream
{
	coDECLARE_SUPER(coOutputStream);
public:
};

inline coBinaryOutputStream& operator<<(coBinaryOutputStream& stream, coBool v)
{
	stream.Write(v);
	return stream;
}

inline coBinaryOutputStream& operator<<(coBinaryOutputStream& stream, coFloat v)
{
	stream.Write(reinterpret_cast<const coByte*>(&v), sizeof(v));
	return stream;
}

inline coBinaryOutputStream& operator<<(coBinaryOutputStream& stream, coUint8 v)
{
	stream.Write(v);
	return stream;
}

inline coBinaryOutputStream& operator<<(coBinaryOutputStream& stream, coUint32 v)
{
	stream.Write(reinterpret_cast<const coByte*>(&v), sizeof(v));
	return stream;
}

inline coBinaryOutputStream& operator<<(coBinaryOutputStream& stream, coUint64 v)
{
	stream.Write(reinterpret_cast<const coByte*>(&v), sizeof(v));
	return stream;
}

template <class T>
inline coBinaryOutputStream& operator<<(coBinaryOutputStream& stream, const coArray<T>& a)
{
	stream << a.count;
	for (const T& v : a)
	{
		stream << v;
	}
	//Write(a.data, a.count * sizeof(T));
	return stream;
}
