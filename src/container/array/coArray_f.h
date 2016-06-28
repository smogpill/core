// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "memory/coMemory_f.h"

template <class T>
coFORCE_INLINE T* coBegin(coArray<T>& _a) { return _a.data; }
template <class T>
coFORCE_INLINE T* coEnd(coArray<T>& _a) { return _a.data + _a.count; }

// STD compat
template <class T>
coFORCE_INLINE T* begin(coArray<T> _a) { return coBegin(_a); }
template <class T>
coFORCE_INLINE T* end(coArray<T> _a) { return coEnd(_a); }

template <class A>
void coResize(A& _this, coUint32 _newCount)
{
	//static_assert(std::is_base_of<coArray, A>::value, "_a should be an array");
	if (_this.count != _newCount)
	{
		coReserve(_this, _newCount);
		_this.count = _newCount;
	}
}

template <class T>
void coClear(coArray<T>& _this)
{
#ifdef coDEBUG
	coAssignAsDeleted(getData(_this), _this.count * sizeof(T));
#endif
	_this.count = 0;
}
