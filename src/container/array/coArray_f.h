// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "memory/coMemory_f.h"
#include "lang/coStdWrapper.h"

template <class T>
coArray<T>::coArray(std::initializer_list<T> _l)
	: data(const_cast<T*>(_l.begin()))
	, count(static_cast<coUint32>(_l.size()))
{
}

#define coARRAY_SIZE(_array_) (sizeof(_array_) / sizeof(_array[0]))

template <class T> coFORCE_INLINE T* coBegin(coArray<T>& _a) { return _a.data; }
template <class T> coFORCE_INLINE const T* coBegin(const coArray<T>& _a) { return _a.data; }
template <class T> coFORCE_INLINE T* coEnd(coArray<T>& _a) { return _a.data + _a.count; }
template <class T> coFORCE_INLINE const T* coEnd(const coArray<T>& _a) { return _a.data + _a.count; }
template <class T> coFORCE_INLINE T& coBack(coArray<T>& _a) { coASSERT(_a.count); return _a.data[_a.count - 1]; }
template <class T> coFORCE_INLINE const T& coBack(const coArray<T>& _a) { coASSERT(_a.count); return _a.data[_a.count - 1]; }

template <class A>
void coResize(A& _this, coUint32 _newCount)
{
	//static_assert(std::is_base_of<coArray, A>::value, "_a should be an array");
	if (_this.count != _newCount)
	{
		coReserve(_this, _newCount);
		_this.count = _newCount;
	}
}

template <class T>
void coClear(coArray<T>& _this)
{
#ifdef coDEBUG
	coAssignAsDeleted(_this.data, _this.count * sizeof(T));
#endif
	_this.count = 0;
}

template <class T>
void coDeleteElementsAndClear(coArray<T>& _this)
{
	for (T e : _this)
		delete e;
	coClear(_this);
}

template <class T, class CompareFunctor>
void coSort(coArray<T>& _this, CompareFunctor _cmp)
{
	if (_this.count == 0)
		return;

	// QuickSort algorithm with:
	// - Hoare Partitioning (faster);
	// - Recursion removed;
	// - Small stack size (to avoid dynamic allocs).

	T* data = _this.data;
	auto HoarePartition = [&](coUint _l, coUint _r) -> coUint
	{
		const T& pivot = data[_l];
		coUint i = _l - 1;
		coUint j = _r + 1;
		for (;;)
		{
			do
			{
				++i;
			} while (_cmp(data[i], pivot));

			do
			{
				--j;
			} while (_cmp(pivot, data[j]));

			if (i >= j)
				return j;

			coSwap(data[i], data[j]);
		}
	};

	static const coUint maxDepth = sizeof(_this.count) * 8; // max of log(n)
	coUint leftStack[maxDepth];
	coUint rightStack[maxDepth];
	leftStack[0] = 0;
	rightStack[0] = _this.count - 1;
	coUint top = 0;
	while (top)
	{
		const coUint l = leftStack[top];
		const coUint r = rightStack[top];
		--top;

		const coUint pivot = HoarePartition(l, r);
		const coUint l0 = l;
		const coUint r0 = pivot;
		const coUint l1 = pivot + 1;
		const coUint r1 = r;
		const coUint sz0 = r0 - l0;
		const coUint sz1 = r1 - l1;
		if (sz0 < sz1) // We want to process the smaller part first to ensure log(n) stack size.
		{
			if (sz1)
			{
				++top;
				coASSERT(top < maxDepth);
				leftStack[top] = l1;
				rightStack[top] = r1;
			}

			if (sz0)
			{
				++top;
				coASSERT(top < maxDepth);
				leftStack[top] = l0;
				rightStack[top] = r0;
			}
		}
		else
		{
			if (sz0)
			{
				++top;
				coASSERT(top < maxDepth);
				leftStack[top] = l0;
				rightStack[top] = r0;
			}
			if (sz1)
			{
				++top;
				coASSERT(top < maxDepth);
				leftStack[top] = l1;
				rightStack[top] = r1;
			}
		}
	}
}

template <class T>
void coSort(coArray<T>& _this)
{
	auto defaultCompare = [](const T& _a, const T& _b) { return _a < _b; };
	coSort(_this, defaultCompare);
}

template <class T, class CompareFunctor>
coBool coIsSorted(const coArray<T>& _this, CompareFunctor _cmp)
{
	const T* data = _this.data;
	for (coUint i = 1; i < _this.count; ++i)
	{
		if (_cmp(data[i], data[i - 1]))
			return false;
	}
	return true;
}

template <class T>
coBool coIsSorted(const coArray<T>& _this)
{
	auto defaultCompare = [](const T& _a, const T& _b) { return _a < _b; };
	return coIsSorted(_this, defaultCompare);
}
