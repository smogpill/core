// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "container/pch.h"
#include "container/string/coConstString_f.h"
#include "container/array/coDynamicArray_f.h"

coConstString::coConstString(const coChar* _s)
	: coConstArray(_s, _s ? static_cast<coUint32>(::strlen(_s)) : 0)
{

}

coConstString::coConstString(const coChar* _s, coUint32 _size)
	: coConstArray(_s, _size)
{

}

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
