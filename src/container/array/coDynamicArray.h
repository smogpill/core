// Copyright(c) 2016-2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "container/array/coArray.h"
#include "memory/allocator/coAllocator.h"
#include <lang/reflect/coTypeDecl.h>
class coBinaryInputStream;

template <class T>
class coDynamicArray : public coArray<T>
{
	using Base = coArray<T>;
	//coDECLARE_BASE(coArray<T>);
	coDECLARE_CLASS_NO_POLYMORPHISM(coDynamicArray);
public:
	coDynamicArray() = default;
	template <coUint N>
	explicit coDynamicArray(const T(&_a)[N]);
	~coDynamicArray()
	{
		coAllocator::GetHeap()->FreeAligned(this->data);
	}
	explicit coDynamicArray(std::initializer_list<T> _l);

	coUint32 capacity = 0;

	explicit coDynamicArray(const coArray<T>&);
	explicit coDynamicArray(const coDynamicArray<T>&);
	coDynamicArray(coDynamicArray<T>&&);
	coDynamicArray<T>& operator=(const coArray<T>&);
	coDynamicArray<T>& operator=(const coArray<const T>&);
	coDynamicArray<T>& operator=(const coDynamicArray<T>&); // Weird that we need this, the compiler does not like the coArray<> version.
	void Read(coBinaryInputStream& stream);
};

