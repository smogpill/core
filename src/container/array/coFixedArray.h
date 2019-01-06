// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coArray.h"

template <class T, coUint32 CAPACITY>
class coFixedArray : public coArray<T>
{
public:
	coFixedArray() : coArray<T>(fixedBuffer, 0){}

private:
	T fixedBuffer[CAPACITY];
};
