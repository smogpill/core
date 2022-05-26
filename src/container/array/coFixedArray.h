// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coArray.h"

template <class T, coUint32 CAPACITY>
class coFixedArray : public coArray<T>
{
	typedef coFixedArray<T, CAPACITY> Self;
public:
	enum
	{
		capacity = CAPACITY,
	};
	coFORCE_INLINE coFixedArray() : coArray<T>(reinterpret_cast<T*>(fixedBuffer), 0) {}
	coFORCE_INLINE coFixedArray(const coArray<T>& other) { *this = other; }
	//coFixedArray(const Self& other) : coFixedArray(static_cast<const coArray<T>&>(other)) {}

	Self& operator=(const Self& other) { return operator=(static_cast<const coArray<T>&>(other)); }
	Self& operator=(const coArray<T>& other);

private:
	alignas(alignof(T)) coByte fixedBuffer[CAPACITY*sizeof(T)];
};

template <class T, coUint32 C>
coFixedArray<T,C>& coFixedArray<T,C>::operator=(const coArray<T>& other)
{
	coASSERT(C >= other.count);
	coMemCopy(this->data, other.data, other.count);
	count = other.count;
	return *this;
}
