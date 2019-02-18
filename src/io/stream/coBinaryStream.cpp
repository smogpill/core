// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/stream/coBinaryStream.h"
#include "io/stream/coStreamBuffer.h"

void coBinaryStream::Write(const coByte* _data, coUint _size8)
{
	buffer->Write(_data, _size8);
}

void coBinaryStream::Read(coByte* data, coUint size8)
{
	buffer->Read(data, size8);
}

coBinaryStream& coBinaryStream::operator<<(coFloat v)
{
	buffer->Write(reinterpret_cast<const coByte*>(&v), sizeof(v));
	return *this;
}

coBinaryStream& coBinaryStream::operator<<(coUint32 v)
{
	buffer->Write(reinterpret_cast<const coByte*>(&v), sizeof(v));
	return *this;
}

coBinaryStream& coBinaryStream::operator >> (coUint32 v)
{
	buffer->Read(reinterpret_cast<coByte*>(&v), sizeof(v));
	return *this;
}

coBinaryStream& coBinaryStream::operator >> (coFloat v)
{
	buffer->Read(reinterpret_cast<coByte*>(&v), sizeof(v));
	return *this;
}
