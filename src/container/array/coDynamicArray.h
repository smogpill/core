// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coArray.h"

class coAllocator;

template <class T>
class coDynamicArray : public coArray<T>
{
	typedef coArray<T> Super;
public:
	coDynamicArray();
	explicit coDynamicArray(coAllocator& _allocator);
	template <coUint N>
	explicit coDynamicArray(const T(&_a)[N]);
	~coDynamicArray();
	explicit coDynamicArray(std::initializer_list<T> _l);

	coUint32 capacity;
	coAllocator* allocator;

	explicit coDynamicArray(const coArray<T>&);
	explicit coDynamicArray(const coDynamicArray<T>&);
	coDynamicArray(coDynamicArray<T>&&);
	coDynamicArray<T>& operator=(const coArray<T>&);
	coDynamicArray<T>& operator=(const coArray<const T>&);
	coDynamicArray<T>& operator=(const coDynamicArray<T>&); // Weird that we need this, the compiler does not like the coArray<> version.
};
