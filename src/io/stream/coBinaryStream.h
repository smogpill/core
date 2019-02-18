// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "io/stream/coStream.h"

class coBinaryStream : public coStream
{
	coDECLARE_SUPER(coStream);
public:
	coBinaryStream(coStreamBuffer& buffer) : Super(buffer){}

	void Write(const coByte* data, coUint size8);
	void Read(coByte* data, coUint size8);

	coBinaryStream& operator<<(coFloat);
	coBinaryStream& operator<<(coUint32);

	coBinaryStream& operator>>(coFloat);
	coBinaryStream& operator>>(coUint32);
};
