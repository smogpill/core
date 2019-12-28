// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "io/stream/coInputStream.h"

class coBinaryInputStream final : public coInputStream
{
	coDECLARE_SUPER(coInputStream);
public:
	coBinaryInputStream(const coArray<coByte>& buffer);

	coBinaryInputStream& operator>> (coFloat&);
	coBinaryInputStream& operator>> (coUint32&);
	template <class T>
	coBinaryInputStream& operator>>(coDynamicArray<T>&);
};

inline coBinaryInputStream& coBinaryInputStream::operator >> (coUint32& v)
{
	Read(reinterpret_cast<coByte*>(&v), sizeof(v));
	return *this;
}

inline coBinaryInputStream& coBinaryInputStream::operator >> (coFloat& v)
{
	Read(reinterpret_cast<coByte*>(&v), sizeof(v));
	return *this;
}

template <class T>
inline coBinaryInputStream& coBinaryInputStream::operator>>(coDynamicArray<T>& v)
{
	coUint32 count;
	*this >> count;
	coClear(v);
	coResize(v, count);
	Read(v.data, count * sizeof(T));
	return *this;
}
