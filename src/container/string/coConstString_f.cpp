// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "container/pch.h"
#include "container/string/coConstString_f.h"
#include "container/string/coChar_f.h"
#include "container/array/coDynamicArray_f.h"
#include "container/array/coConstArray_f.h"
#include "debug/log/coAssert.h"
#include "math/hash/coHash_f.h"

coBool operator==(const coConstString& _a, const coConstString& _b)
{
	if (_a.count != _b.count)
		return false;

	for (coUint i = 0; i < _a.count; ++i)
	{
		if (_a.data[i] != _b.data[i])
		{
			return false;
		}
	}
	return true;
}

coBool operator!=(const coConstString& _a, const coConstString& _b)
{
	return !operator==(_a, _b);
}

void coSplit(coDynamicArray<coConstString>& _out, const coConstString& _input, const coConstString& _delimiters)
{
	coClear(_out);
	const coChar* c = _input.data;
	const coChar* d = _delimiters.data;
	coUint startIndex = 0;
	coUint i;
	for (i = 0; i < _input.count; ++i)
	{
		for (coUint j = 0; j < _delimiters.count; ++j)
		{
			if (c[i] == d[j])
			{
				if (i != startIndex)
				{
					coPushBack(_out, coConstString(&c[startIndex], i - startIndex));
				}
				startIndex = i + 1;
				break;
			}
		}
	}
	if (i != startIndex)
		coPushBack(_out, coConstString(&c[startIndex], i - startIndex));
}

coConstString coGetSubStringFromPos(const coConstString& _a, coUint32 _pos)
{
	coASSERT(_pos <= _a.count);
	return coConstString(_a.data + _pos, _a.count - _pos);
}

coConstString coGetSubStringFromSize(const coConstString& _a, coUint32 _size)
{
	coASSERT(_size <= _a.count);
	return coConstString(_a.data, _size < _a.count ? _size : _a.count);
}

coConstString ccoGetSubStringAfterToken(const coConstString& _a, const coConstString& _token)
{
	const coChar* tokenEnd = coEnd(_token);
	coASSERT(_token.data >= _a.data && tokenEnd <= coEnd(_a));
	return coConstString(tokenEnd, static_cast<coUint32>(coIntPtr(coEnd(_a)) - coIntPtr(tokenEnd)));
}

coUint coFindFirstChar(const coConstString& _this, coChar _c)
{
	const coChar* s = _this.data;
	for (coUint i = 0; i < _this.count; ++i)
		if (s[i] == _c)
			return i;
	return _this.count;
}

coUint coFindLastChar(const coConstString& _this, coChar _c)
{
	const coChar* s = _this.data;
	for (coInt i = _this.count - 1; i >= 0; --i)
		if (s[i] == _c)
			return i;
	return _this.count;
}

coBool coAreMemoryOverlapping(const coConstString& _a, const coConstString& _b)
{
	const coChar* charMin = _a.data > _b.data ? _a.data : _b.data;
	const coChar* charMax = coEnd(_a) < coEnd(_b) ? coEnd(_a) : coEnd(_b);
	return charMin <= charMax;
}

coConstString coLeftStrip(const coConstString& _s, const coConstString& _chars)
{
	coInt i;
	for (i = 0; i < static_cast<coInt>(_s.count); ++i)
	{
		coUint j;
		for (j = 0; j < _chars.count; ++j)
		{
			if (_s[i] == _chars[j])
				break;
		}
		if (j == _chars.count)
			break;
	}
	return coGetSubStringFromPos(_s, i);
}

coConstString coRightStrip(const coConstString& _s, const coConstString& _chars)
{
	coInt i;
	for (i = _s.count - 1; i >= 0; --i)
	{
		coUint j;
		for (j = 0; j < _chars.count; ++j)
		{
			if (_s[i] == _chars[j])
				break;
		}
		if (j == _chars.count)
			break;
	}
	return coGetSubStringFromSize(_s, i + 1);
}

coBool coStartsWith(const coConstString& _this, const coConstString& _prefix)
{
	if (_this.count < _prefix.count)
		return false;

	const coChar* data = _this.data;
	const coChar* prefixData = _prefix.data;
	for (coUint i = 0; i < _prefix.count; ++i)
	{
		if (data[i] != prefixData[i])
			return false;
	}
	return true;
}

coBool coEndsWith(const coConstString& this_, const coConstString& suffix)
{
	if (this_.count < suffix.count)
		return false;

	const coChar* data = this_.data;
	const coChar* suffixData = suffix.data;
	for (coUint i = 0, j = this_.count - suffix.count; i < suffix.count; ++i, ++j)
	{
		if (data[j] != suffixData[i])
			return false;
	}
	return true;
}

coUint32 coHash32(const coConstString& _this)
{
	return coHash32(_this.data, _this.count);
}

coUint64 coHash64(const coConstString& _this)
{
	return coHash64(_this.data, _this.count);
}

coUint32 coParseUint32(const coConstString& _this)
{
	coUint32 v = 0;
	coASSERT(_this.count <= 10);
	for (coUint i = 0; i < _this.count; ++i)
	{
		const coChar c = _this.data[i];
		if (coIsDigit(c))
		{
			v = v * 10 + c - '0';
		}
	}
	return v;
}
