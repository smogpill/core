// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coPack.h"
#include "container/array/coDynamicArray_f.h"
#include "container/string/coConstString.h"

class coConstString;

inline void _coPush(coPack& this_, const void* data, coUint32 size)
{
	coPushBackArray(this_.data, coArray<const coByte>(reinterpret_cast<const coByte*>(data), size));
}

template <class T>
inline void _coPush(coPack& this_, const T& data)
{
	coPushBackArray(this_.data, coArray<coByte>(reinterpret_cast<coByte*>(data), sizeof(T)));
}

inline void coPushBlock(coPack& this_, coUint16 key)
{
	_coPush(this_, key);
}

inline void coPopBlock(coPack& this_)
{

}

template <class T>
inline void coPushValue(coPack& this_, coUint16 key, const T& x)
{
	_coPush(this_, key);
	_coPush(this_, x);
}

inline void coPushValue(coPack& this_, coUint16 key, const coConstString& s)
{
	_coPush(this_, key);
	_coPush(this_, s.data, s.count);
	coPushBack(this_.data, '\0');
}

inline void coPushValue(coPack& this_, coUint16 key, coBool b)
{
	_coPush(this_, key);
	_coPush(this_, b);
}

inline void coPushValue(coPack& this_, coUint16 key, coUint8 x)
{
	_coPush(this_, key);
	_coPush(this_, x);
}

inline void coPushValue(coPack& this_, coUint16 key, coInt8 x)
{
	_coPush(this_, key);
	_coPush(this_, x);
}
