// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "io/stream/coStream.h"

class coStringOutputStream : public coStream
{
	coDECLARE_SUPER(coStream);
public:

	void Write(const coByte* _data, coUint _size8);

	coStringOutputStream& operator<<(coChar);
	coStringOutputStream& operator<<(coInt8);
	coStringOutputStream& operator<<(coInt16);
	coStringOutputStream& operator<<(coInt32);
	coStringOutputStream& operator<<(coInt64);
	coStringOutputStream& operator<<(coUint8);
	coStringOutputStream& operator<<(coUint16);
	coStringOutputStream& operator<<(coUint32);
	coStringOutputStream& operator<<(coUint64);
	coStringOutputStream& operator<<(coBool);
	coStringOutputStream& operator<<(coFloat);
	coStringOutputStream& operator<<(coFloat64);
	coStringOutputStream& operator<<(const coConstString&);
	coStringOutputStream& operator<<(const void*);
	coStringOutputStream& operator<<(coNullPtr);
};
