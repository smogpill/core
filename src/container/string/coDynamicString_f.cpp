// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "container/pch.h"
#include "container/string/coDynamicString_f.h"
#include "container/string/coConstString.h"
#include "container/array/coDynamicArray_f.h"

coDynamicString& operator<<(coDynamicString& _this, const coConstString& _a)
{
	coPushBackArray(_this, _a);
	return _this;
}

coDynamicString& operator<<(coDynamicString& _this, coUint32 _a)
{
	coChar s[16];
	::sprintf_s(s, 16, "%u", _a);
	return _this << coConstString(s);
}

void coNullTerminate(coDynamicString& _this)
{
	if (!coIsNullTerminated(_this))
		coPushBack(_this, '\0');
}

coBool coIsNullTerminated(const coDynamicString& _this)
{
	return _this.count > 0 && _this.data[_this.count - 1] == '\0';
}
