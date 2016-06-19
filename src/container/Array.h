// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/types/baseTypes.h"
#include "lang/cppExtensions.h"

namespace co
{
	class allocator;

	template <class T>
	class ConstArray
	{
	public:
		ConstArray();
		ConstArray(const T* _data, uint32 _size);
		coFORCE_INLINE const T& operator[] (uint32 _i) const { coASSERT(_i < size); return data[_i]; }

		const T* data;
		uint32 size;
	};

	template <class T>
	class Array : public ConstArray<T>
	{
	public:
		Array();
		Array(T* _data, coUint32 _size);
		coFORCE_INLINE T& operator[] (coUint _i) { coASSERT(_i < size); return const_cast<T&>(data[_i]); }
		coFORCE_INLINE const T& operator[] (coUint _i) const { coASSERT(_i < size); return data[_i]; }
	};

	template <class T>
	class DynamicArray : public Array<T>
	{
	public:
		DynamicArray();
		DynamicArray(Allocator& _allocator);
		~DynamicArray();

		uint32 capacity;
		Allocator* allocator;
	};

	template <typename T, uint32 CAPACITY>
	class FixedArray : public Array<T>
	{
	public:
		FixedArray();

	private:
		T fixedBuffer[CAPACITY];
	};
}
