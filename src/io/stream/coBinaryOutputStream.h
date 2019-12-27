// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "io/stream/coOutputStream.h"

class coBinaryOutputStream final : public coOutputStream
{
	coDECLARE_SUPER(coOutputStream);
public:
	coBinaryOutputStream& operator<<(coFloat);
	coBinaryOutputStream& operator<<(coUint32);
	template <class T>
	coBinaryOutputStream& operator<<(const coArray<T>& a);
};

inline coBinaryOutputStream& coBinaryOutputStream::operator<<(coFloat v)
{
	Write(reinterpret_cast<const coByte*>(&v), sizeof(v));
	return *this;
}

inline coBinaryOutputStream& coBinaryOutputStream::operator<<(coUint32 v)
{
	Write(reinterpret_cast<const coByte*>(&v), sizeof(v));
	return *this;
}

template <class T>
inline coBinaryOutputStream& coBinaryOutputStream::operator<<(const coArray<T>& a)
{
	*this << a.count;
	Write(a.data, a.count * sizeof(T));
	return *this;
}
