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
