// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "io/stream/coOutputStream.h"

class coStringOutputStream final : public coOutputStream
{
	coDECLARE_SUPER(coOutputStream);
public:
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
	coStringOutputStream& operator<<(const coChar*);
	coStringOutputStream& operator<<(const void*);
	coStringOutputStream& operator<<(coNullPtr);
	coStringOutputStream& Indent(coUint nb);
};

inline coStringOutputStream& coStringOutputStream::operator<< (coChar _v)
{
	Write(_v);
	return *this;
}

inline coStringOutputStream& coStringOutputStream::operator<< (coInt8 _v)
{
	coChar s[8];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRId8, _v);
	Write(s, len);
	return *this;
}

inline coStringOutputStream& coStringOutputStream::operator<< (coInt16 _v)
{
	coChar s[8];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRId16, _v);
	Write(s, len);
	return *this;
}

inline coStringOutputStream& coStringOutputStream::operator<< (coInt32 _v)
{
	coChar s[16];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRId32, _v);
	Write(s, len);
	return *this;
}

inline coStringOutputStream& coStringOutputStream::operator<< (coInt64 _v)
{
	coChar s[32];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRId64, _v);
	Write(s, len);
	return *this;
}

inline coStringOutputStream& coStringOutputStream::operator<< (coUint8 _v)
{
	coChar s[8];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRIu8, _v);
	Write(s, len);
	return *this;
}

inline coStringOutputStream& coStringOutputStream::operator<< (coUint16 _v)
{
	coChar s[8];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRIu16, _v);
	Write(s, len);
	return *this;
}

inline coStringOutputStream& coStringOutputStream::operator<< (coUint32 _v)
{
	coChar s[16];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRIu32, _v);
	Write(s, len);
	return *this;
}

inline coStringOutputStream& coStringOutputStream::operator<< (coUint64 _v)
{
	coChar s[32];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRIu64, _v);
	Write(s, len);
	return *this;
}

inline coStringOutputStream& coStringOutputStream::operator<< (coBool _v)
{
	const coChar sTrue[] = "true";
	const coChar sFalse[] = "false";
	if (_v)
	{
		Write(reinterpret_cast<const coByte*>(sTrue), coARRAY_SIZE(sTrue) - 1);
	}
	else
	{
		Write(reinterpret_cast<const coByte*>(sFalse), coARRAY_SIZE(sFalse) - 1);
	}
	return *this;
}

inline coStringOutputStream& coStringOutputStream::operator<< (coFloat _v)
{
	coChar s[32];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), coFLOAT_AS_STRING_FORMAT, _v);
	Write(s, len);
	return *this;
}

inline coStringOutputStream& coStringOutputStream::operator<< (coFloat64 _v)
{
	coChar s[64];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), coFLOAT64_AS_STRING_FORMAT, _v);
	Write(s, len);
	return *this;
}

inline coStringOutputStream& coStringOutputStream::operator<< (const coConstString& _s)
{
	Write(_s.data, _s.count);
	return *this;
}

inline coStringOutputStream& coStringOutputStream::operator<<(const coChar* _s)
{
	return operator<<(coConstString(_s));
}

inline coStringOutputStream& coStringOutputStream::operator<< (const void* _v)
{
	coChar s[32];
	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%p", _v);
	Write(s, len);
	return *this;
}

inline coStringOutputStream& coStringOutputStream::operator<< (coNullPtr)
{
	const coChar s[] = "<null>";
	Write(s, coARRAY_SIZE(s) - 1);
	return *this;
}

inline coStringOutputStream& coStringOutputStream::Indent(coUint nb)
{
	for (coUint i = 0; i < nb; ++i)
		Write('\t');
	return *this;
}
