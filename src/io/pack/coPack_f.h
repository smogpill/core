// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coPack.h"
#include "container/array/coDynamicArray_f.h"
#include "container/string/coConstString.h"

class coConstString;

inline void _coPush(coPack& this_, const void* data, coUint32 size)
{
	coPushBackArray(this_.buffer, coArray<const coByte>(reinterpret_cast<const coByte*>(data), size));
}

template <class T>
inline void _coPush(coPack& this_, const T& data)
{
	coPushBackArray(this_.buffer, coArray<const coByte>(reinterpret_cast<const coByte*>(&data), sizeof(T)));
}

inline void coPushBlock(coPack& this_, coUint8 id)
{
	_coPush(this_, id);
}

inline void coPopBlock(coPack& this_)
{

}

template <class T>
inline void coPushValue(coPack& this_, coUint8 id, const T& x)
{
	_coPush(this_,id);
	_coPush(this_, x);
}

inline void coPushValue(coPack& this_, coUint8 id, const coConstString& s)
{
	_coPush(this_, id);
	_coPush(this_, s.data, s.count);
	coPushBack(this_.buffer, '\0');
}

inline void coPushValue(coPack& this_, coUint8 id, coBool b)
{
	_coPush(this_, id);
	_coPush(this_, b);
}

inline void coPushValue(coPack& this_, coUint8 id, coUint8 x)
{
	_coPush(this_, id);
	_coPush(this_, x);
}

inline void coPushValue(coPack& this_, coUint8 id, coInt8 x)
{
	_coPush(this_, id);
	_coPush(this_, x);
}
