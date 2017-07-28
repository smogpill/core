// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "memory/pch.h"
#include "memory/coMemory_f.h"

void coFillAsDeleted(void* _ptr, coUint _size8)
{
	coByte* ptr = static_cast<coByte*>(_ptr);
	while (_size8 >= 4)
	{
		*static_cast<coUint32*>(_ptr) = 0xfefefefe;
		ptr += 4;
		_size8 -= 4;
	}
	while (_size8)
	{
		*ptr = 0xfe;
		++ptr;
		--_size8;
	}
}
