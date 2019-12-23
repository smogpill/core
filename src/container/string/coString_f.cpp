// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "container/pch.h"
#include "container/string/coString_f.h"
#include "container/string/coString.h"
#include "container/string/coChar_f.h"

void coRemoveMultiples(coString& this_, const coChar c_)
{
	const coUint count = this_.count;
	const coUint countMinus1 = count - 1;
	coUint j = 0;
	for (coUint i = 0; i < count; ++i)
	{
		const coChar c = this_.data[i];
		if (c == c_)
		{
			while (i < countMinus1 && this_.data[i + 1] == c_)
			{
				++i;
			}
		}
		this_.data[j] = c;
		++j;
	}
	coASSERT(j <= this_.count);
	this_.count = j;
}

void coRemoveWhitespace(coString& this_)
{
	coUint offset = 0;
	const coUint count = this_.count;
	for (coUint i = 0; i < count; ++i)
	{
		const coChar c = this_.data[i];
		if (!coIsWhitespace(c))
		{
			this_.data[offset++] = c;
		}
	}
	this_.count = offset;
}
