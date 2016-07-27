// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/stream/coStringOutputStream.h"
#include "io/stream/coStreamBuffer.h"

void coStringOutputStream::Write(const coByte* _data, coUint _size8)
{
	buffer->Write(_data, _size8);
}

coStringOutputStream& coStringOutputStream::operator<< (coChar _v)
{
	buffer->Write(_v);
	return *this;
}

coStringOutputStream& coStringOutputStream::operator<< (coInt8 _v)
{
	coChar s[8];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRId8, _v);
	buffer->Write(reinterpret_cast<const coByte*>(s), len);
	return *this;
}

coStringOutputStream& coStringOutputStream::operator<< (coInt16 _v)
{
	coChar s[8];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRId16, _v);
	buffer->Write(reinterpret_cast<const coByte*>(s), len);
	return *this;
}

coStringOutputStream& coStringOutputStream::operator<< (coInt32 _v)
{
	coChar s[16];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRId32, _v);
	buffer->Write(reinterpret_cast<const coByte*>(s), len);
	return *this;
}

coStringOutputStream& coStringOutputStream::operator<< (coInt64 _v)
{
	coChar s[32];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRId64, _v);
	buffer->Write(reinterpret_cast<const coByte*>(s), len);
	return *this;
}

coStringOutputStream& coStringOutputStream::operator<< (coUint8 _v)
{
	coChar s[8];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRIu8, _v);
	buffer->Write(reinterpret_cast<const coByte*>(s), len);
	return *this;
}

coStringOutputStream& coStringOutputStream::operator<< (coUint16 _v)
{
	coChar s[8];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRIu16, _v);
	buffer->Write(reinterpret_cast<const coByte*>(s), len);
	return *this;
}

coStringOutputStream& coStringOutputStream::operator<< (coUint32 _v)
{
	coChar s[16];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRIu32, _v);
	buffer->Write(reinterpret_cast<const coByte*>(s), len);
	return *this;
}

coStringOutputStream& coStringOutputStream::operator<< (coUint64 _v)
{
	coChar s[32];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRIu64, _v);
	buffer->Write(reinterpret_cast<const coByte*>(s), len);
	return *this;
}

coStringOutputStream& coStringOutputStream::operator<< (coBool _v)
{
	const coChar sTrue[] = "true";
	const coChar sFalse[] = "false";
	if (_v)
	{
		buffer->Write(reinterpret_cast<const coByte*>(sTrue), coARRAY_SIZE(sTrue) - 1);
	}
	else
	{
		buffer->Write(reinterpret_cast<const coByte*>(sFalse), coARRAY_SIZE(sFalse) - 1);
	}
	return *this;
}

coStringOutputStream& coStringOutputStream::operator<< (coFloat _v)
{
	coChar s[32];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%f", _v);
	buffer->Write(reinterpret_cast<const coByte*>(s), len);
	return *this;
}

coStringOutputStream& coStringOutputStream::operator<< (coFloat64 _v)
{
	coChar s[64];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%f", _v);
	buffer->Write(reinterpret_cast<const coByte*>(s), len);
	return *this;
}

coStringOutputStream& coStringOutputStream::operator<< (const coConstString& _s)
{
	buffer->Write(reinterpret_cast<const coByte*>(_s.data), _s.count);
	return *this;
}

coStringOutputStream& coStringOutputStream::operator<< (const void* _v)
{
	coChar s[32];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%p", _v);
	buffer->Write(reinterpret_cast<const coByte*>(s), len);
	return *this;
}

coStringOutputStream& coStringOutputStream::operator<< (coNullPtr)
{
	const coChar s[] = "<null>";
	buffer->Write(reinterpret_cast<const coByte*>(s), coARRAY_SIZE(s)-1);
	return *this;
}
