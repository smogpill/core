// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "container/pch.h"
#include "container/array/coDynamicArray_f.h"
#include "math/scalar/coUint32_f.h"

coUint32 _coComputeBestArrayCapacity(coUint32 _capacity)
{
	const coUint32 newCapacity = _capacity >= 16u ? coNextPowerOf2(_capacity) : 16u;
	coASSERT(newCapacity >= _capacity);
	return newCapacity;
}
