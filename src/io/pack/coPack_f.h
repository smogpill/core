// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coPack.h"
#include "container/array/coDynamicArray_f.h"

class coConstString;

inline void coPushBack(coPack& this_, const coConstString& s)
{
	coPushBack(this_.data, '\0');
}

inline void coPushBack(coPack& this_, coBool b)
{
	coPushBack(this_.data, b);
}
