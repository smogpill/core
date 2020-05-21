// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "container/pch.h"
#include "container/string/coDynamicString_f.h"
#include "container/string/coConstString.h"
#include "container/string/coConstString16.h"
#include "container/array/coDynamicArray_f.h"
#include "lang/reflect/coNumericLimits.h"

coDynamicString& operator<<(coDynamicString& _this, const coConstString& _a)
{
	coPushBackArray(_this, _a);
	return _this;
}

coDynamicString& operator<<(coDynamicString& _this, coChar _a)
{
	coPushBack(_this, _a);
	return _this;
}

coDynamicString& operator<<(coDynamicString& _this, coInt8 _a)
{
	coChar s[4];
	::sprintf_s(s, coARRAY_SIZE(s), "%" PRIi8, _a);
	return _this << coConstString(s);
}

coDynamicString& operator<<(coDynamicString& _this, coInt16 _a)
{
	coChar s[16];
	::sprintf_s(s, coARRAY_SIZE(s), "%" PRIi16, _a);
	return _this << coConstString(s);
}

coDynamicString& operator<<(coDynamicString& _this, coInt32 _a)
{
	coChar s[16];
	::sprintf_s(s, coARRAY_SIZE(s), "%" PRIi32, _a);
	return _this << coConstString(s);
}

coDynamicString& operator<<(coDynamicString& _this, coInt64 _a)
{
	coChar s[128];
	::sprintf_s(s, coARRAY_SIZE(s), "%" PRIi64, _a);
	return _this << coConstString(s);
}

coDynamicString& operator<<(coDynamicString& _this, coUint8 _a)
{
	coChar s[4];
	::sprintf_s(s, coARRAY_SIZE(s), "%" PRIu8, _a);
	return _this << coConstString(s);
}

coDynamicString& operator<<(coDynamicString& _this, coUint16 _a)
{
	coChar s[16];
	::sprintf_s(s, coARRAY_SIZE(s), "%" PRIu16, _a);
	return _this << coConstString(s);
}

coDynamicString& operator<<(coDynamicString& _this, coUint32 _a)
{
	coChar s[16];
	::sprintf_s(s, coARRAY_SIZE(s), "%" PRIu32, _a);
	return _this << coConstString(s);
}

coDynamicString& operator<<(coDynamicString& _this, coUint64 _a)
{
	coChar s[128];
	::sprintf_s(s, coARRAY_SIZE(s), "%" PRIu64, _a);
	return _this << coConstString(s);
}

coDynamicString& operator<<(coDynamicString& _this, coFloat _a)
{
	coChar s[128];
	::sprintf_s(s, coARRAY_SIZE(s), coFLOAT_AS_STRING_FORMAT, _a);
	return _this << coConstString(s);
}

void coNullTerminate(coDynamicString& _this)
{
	if (!coIsNullTerminated(_this))
	{
		coReserve(_this, _this.count + 1);
		_this.data[_this.count] = '\0';
	}
}

coBool coIsNullTerminated(const coDynamicString& _this)
{
	return _this.capacity > _this.count && _this.data[_this.count] == '\0';
}

void coJoin(coDynamicString& _this, const coConstString& _a, const coConstString& _b, coChar _separator)
{
	coASSERT(!coAreMemoryOverlapping(_this, _a));
	coASSERT(!coAreMemoryOverlapping(_this, _b));
	coConstString stripped0;
	coConstString stripped1;
	coConstString sep(&_separator, 1);
	coRightStrip(stripped0, _a, sep);
	coLeftStrip(stripped1, _b, sep);
	coUint32 newSize = stripped0.count + stripped1.count + sizeof(_separator);
	coReserve(_this, newSize);
	_this.count = newSize;
	coMemCopy(_this.data, stripped0.data, stripped0.count);
	_this[stripped0.count] = _separator;
	coMemCopy(&_this.data[stripped0.count + 1], stripped1.data, stripped1.count);
}

void coSetFromWide(coDynamicString& _this, const coConstString16& _other)
{
	std::wstring_convert<std::codecvt<coWideChar, coChar, std::mbstate_t>, coWideChar> conv16;
	std::string s = conv16.to_bytes(_other.data, _other.data + _other.count);
	coASSERT(s.length() < coNumericLimits<decltype(_this.count)>::Max());
	_this = coConstString(s.data(), static_cast<coUint32>(s.length()));
}

void coSwap(coDynamicString& _this, coDynamicString& _other)
{
	coSwapMemory(_this, _other);
}
