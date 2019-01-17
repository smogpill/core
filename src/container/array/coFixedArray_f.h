// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coArray_f.h>
#include <container/array/coFixedArray.h>
#include <debug/log/coAssert.h>

template <class T, coUint32 C>
void coReserve(coFixedArray<T, C>& _this, coUint32 _desiredCount)
{
	(void)_this;
	(void)_desiredCount;
	coASSERT(_desiredCount <= _this.capacity);
}

template <class T, coUint32 C>
T& coPushBack(coFixedArray<T, C>& _this, const T& _val)
{
	coASSERT(_this.count < C);
	T& e = _this.data[_this.count++];
	e = _val;
	coASSERT(_this.count <= C);
	return e;
}
