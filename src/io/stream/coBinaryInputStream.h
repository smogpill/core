// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "io/stream/coInputStream.h"

class coBinaryInputStream final : public coInputStream
{
	coDECLARE_SUPER(coInputStream);
public:
	coBinaryInputStream& operator>> (coFloat&);
	coBinaryInputStream& operator>> (coUint32&);
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

