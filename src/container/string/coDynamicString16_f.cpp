// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "container/pch.h"
#include "container/string/coDynamicString16_f.h"
#include "container/string/coDynamicString16.h"
#include "container/string/coConstString.h"
#include "container/string/coConstString16.h"
#include "container/array/coDynamicArray_f.h"
#include "lang/reflect/coNumericLimits.h"

coDynamicString16& operator<<(coDynamicString16& _this, const coConstString16& _a)
{
	coPushBackArray(_this, _a);
	return _this;
}

void coSetFromUTF8(coDynamicString16& this_, const coConstString& other)
{
#ifdef coMSWINDOWS
	const coUint nbChars = MultiByteToWideChar(CP_UTF8, 0, other.data, other.count, nullptr, 0);
	coResize(this_, nbChars);
	MultiByteToWideChar(CP_UTF8, 0, other.data, other.count, this_.data, nbChars);
#else
#error "Not available yet"
#endif
}

void coNullTerminate(coDynamicString16& _this)
{
	if (!coIsNullTerminated(_this))
	{
		coReserve(_this, _this.count + 1);
		_this.data[_this.count] = '\0';
	}
}

coBool coIsNullTerminated(const coDynamicString16& _this)
{
	return _this.capacity > _this.count && _this.data[_this.count] == L'\0';
}

void coSwap(coDynamicString16& _this, coDynamicString16& _other)
{
	coSwapMemory(_this, _other);
}
