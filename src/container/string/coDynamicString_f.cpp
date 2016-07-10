// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "container/pch.h"
#include "container/string/coDynamicString_f.h"
#include "container/string/coConstString.h"
#include "container/array/coDynamicArray_f.h"

coDynamicString::coDynamicString(const coConstString& _s)
	: coDynamicString()
{
	operator=(_s);
}

coDynamicString::coDynamicString(coAllocator& _allocator)
	: coDynamicArray<coChar>(_allocator)
{

}

coDynamicString& coDynamicString::operator=(const coConstString& _s)
{
	coReserve(*this, _s.count);
	coMemCopy(data, _s.data, _s.count);
	count = _s.count;
	return *this;
}

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

// coBool operator==(coDynamicString& _a, const coConstString& _b)
// {
// 	return static_cast<const coConstString&>(_a) == _b;
// }
// 
// coBool operator!=(coDynamicString& _a, const coConstString& _b)
// {
// 	return static_cast<const coConstString&>(_a) != _b;
// }

void coNullTerminate(coDynamicString& _this)
{
	if (_this.count == 0 || _this.data[_this.count - 1] != '\0')
	{
		coPushBack(_this, '\0');
	}
}
