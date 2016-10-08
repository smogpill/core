// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "container/pch.h"
#include "container/string/coString_f.h"
#include "container/string/coString.h"

void coRemoveMultiples(coString& _this, const coChar _c)
{
	const coUint count = _this.count;
	const coUint countMinus1 = count - 1;
	coUint j = 0;
	for (coUint i = 0; i < count; ++i)
	{
		const coChar c = _this.data[i];
		if (c == _c)
		{
			while (i < countMinus1 && _this.data[i + 1] == _c)
			{
				++i;
			}
		}
		_this.data[j] = c;
		++j;
	}
	coASSERT(j <= _this.count);
	_this.count = j;
}
