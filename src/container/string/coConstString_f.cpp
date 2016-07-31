// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "container/pch.h"
#include "container/string/coConstString_f.h"
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

void coGetSubStringFromPos(coConstString& _out, const coConstString& _a, coUint32 _pos)
{
	coASSERT(_pos <= _a.count);
	_out.data = _a.data + _pos;
	_out.count = _a.count - _pos;
}
void coGetSubStringFromSize(coConstString& _out, const coConstString& _a, coUint32 _size)
{
	coASSERT(_size <= _a.count);
	_out.data = _a.data;
	_out.count = _size < _a.count ? _size : _a.count;
}

void ccoGetSubStringAfterToken(coConstString& _out, const coConstString& _a, const coConstString& _token)
{
	coASSERT(_token.data >= _a.data && coEnd(_token) <= coEnd(_a));
	_out.data = coEnd(_token);
	_out.count = static_cast<coUint32>(coIntPtr(coEnd(_a)) - coIntPtr(_out.data));
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

void coLeftStrip(coConstString& _out, const coConstString& _s, const coConstString& _chars)
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
	coGetSubStringFromPos(_out, _s, i);
}

void coRightStrip(coConstString& _out, const coConstString& _s, const coConstString& _chars)
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
	coGetSubStringFromSize(_out, _s, i + 1);
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

coUint32 coHash32(const coConstString& _this)
{
	return coHash32(_this.data, _this.count);
}

coUint64 coHash64(const coConstString& _this)
{
	return coHash64(_this.data, _this.count);
}
