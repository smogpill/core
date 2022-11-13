// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "io/stream/coInputStream.h"

class coStringInputStream final : public coInputStream
{
	coDECLARE_BASE(coInputStream);
public:
	coStringInputStream(const coArray<coByte>& buffer);
	void PassHorizontalWhitespace();
	void PassWhitespace();
	void PassLine();
	coBool IsEndOfLine() const;
	coChar ReadChar() { return pos < buffer.count ? buffer.data[pos++] : '\0'; }
	coChar GetChar() const { return pos < buffer.count ? buffer.data[pos] : '\0'; }
	friend coStringInputStream& operator >> (coStringInputStream& stream, coFloat& v);
	friend coStringInputStream& operator >> (coStringInputStream& stream, coUint32& v);
// 	coStringInputStream& operator >> (coChar&);
// 	coStringInputStream& operator >> (coInt8&);
// 	coStringInputStream& operator >> (coInt16&);
// 	coStringInputStream& operator >> (coInt32&);
// 	coStringInputStream& operator >> (coInt64&);
// 	coStringInputStream& operator >> (coUint8&);
// 	coStringInputStream& operator >> (coUint16&);
// 	coStringInputStream& operator >> (coUint32&);
// 	coStringInputStream& operator >> (coUint64&);
// 	coStringInputStream& operator >> (coBool&);
//	coStringInputStream& operator >> (coFloat&);
// 	coStringInputStream& operator >> (coFloat64&);
// 	coStringInputStream& operator >> (coDynamicString&);
};

// inline coStringInputStream& coStringInputStream::operator >> (coChar& _v)
// {
// 	Read(_v);
// 	return *this;
// }
// 
// inline coStringInputStream& coStringInputStream::operator >> (coInt8& _v)
// {
// 	coChar s[8];
// 	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRId8, _v);
// 	Write(s, len);
// 	return *this;
// }
// 
// inline coStringInputStream& coStringInputStream::operator >> (coInt16& _v)
// {
// 	coChar s[8];
// 	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRId16, _v);
// 	Write(s, len);
// 	return *this;
// }
// 
// inline coStringInputStream& coStringInputStream::operator >> (coInt32& _v)
// {
// 	coChar s[16];
// 	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRId32, _v);
// 	Write(s, len);
// 	return *this;
// }
// 
// inline coStringInputStream& coStringInputStream::operator >> (coInt64& _v)
// {
// 	coChar s[32];
// 	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRId64, _v);
// 	Write(s, len);
// 	return *this;
// }
// 
// inline coStringInputStream& coStringInputStream::operator >> (coUint8& _v)
// {
// 	coChar s[8];
// 	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRIu8, _v);
// 	Write(s, len);
// 	return *this;
// }
// 
// inline coStringInputStream& coStringInputStream::operator >> (coUint16& _v)
// {
// 	coChar s[8];
// 	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRIu16, _v);
// 	Write(s, len);
// 	return *this;
// }
// 
// inline coStringInputStream& coStringInputStream::operator >> (coUint32& _v)
// {
// 	coChar s[16];
// 	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRIu32, _v);
// 	Write(s, len);
// 	return *this;
// }
// 
// inline coStringInputStream& coStringInputStream::operator >> (coUint64& _v)
// {
// 	coChar s[32];
// 	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), "%" PRIu64, _v);
// 	Write(s, len);
// 	return *this;
// }
// 
// inline coStringInputStream& coStringInputStream::operator >> (coBool& _v)
// {
// 	const coChar sTrue[] = "true";
// 	const coChar sFalse[] = "false";
// 	if (_v)
// 	{
// 		Write(reinterpret_cast<const coByte*>(sTrue), coARRAY_SIZE(sTrue) - 1);
// 	}
// 	else
// 	{
// 		Write(reinterpret_cast<const coByte*>(sFalse), coARRAY_SIZE(sFalse) - 1);
// 	}
// 	return *this;
// }
// 

coStringInputStream& operator >> (coStringInputStream& stream, coUint32& v);
coStringInputStream& operator >> (coStringInputStream& stream, coFloat& v);

// 
// inline coStringInputStream& coStringInputStream::operator >> (coFloat64& _v)
// {
// 	coChar s[64];
// 	const coInt len = ::sprintf_s(s, coARRAY_SIZE(s), coFLOAT64_AS_STRING_FORMAT, _v);
// 	Write(s, len);
// 	return *this;
// }
// 
// inline coStringInputStream& coStringInputStream::operator >> (coDynamicString& _s)
// {
// 	Write(_s.data, _s.count);
// 	return *this;
// }
