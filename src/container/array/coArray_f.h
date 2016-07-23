// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "memory/coMemory_f.h"
#include "lang/coStdWrapper.h"

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

// template <class T, class CompareFunctor>
// void _coQuickSort(T* _data, CompareFunctor _cmp)
// {
// 	auto Partition = [&](coUint _l, coUint _r) -> coUint
// 	{
// 		const T& pivot = _this.data[_l];
// 		coUint i = _l - 1;
// 		coUint j = _r + 1;
// 		for (;;)
// 		{
// 			do
// 			{
// 				++i;
// 			} while (_cmp(_this.data[i], pivot));
// 
// 			do
// 			{
// 				--j;
// 			} while (_cmp(pivot, _this.data[j]));
// 
// 			if (i >= j)
// 				return j;
// 
// 			coSwap(_this.data[i], _this.data[j]);
// 		}
// 	};
// 
// 	if (_l < _r)
// 	{
// 		const coUint pivot = Partition(_l, _r);
// 		QuickSort(_l, pivot);
// 		QuickSort(pivot + 1, _r);
// 	}
// 
// 	QuickSort(0, _this.count - 1);
// }

template <class T, class CompareFunctor>
void coSort(coArray<T>& _this, CompareFunctor _cmp)
{
	if (_this.count == 0)
		return;

	// QuickSort algorithm

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
		const coUint pl0 = l;
		const coUint pr0 = pivot;
		const coUint pl1 = pivot + 1;
		const coUint pr1 = r;
		const coUint sz0 = pr0 - pl0;
		const coUint sz1 = pr1 - pl1;
		if (sz0 < sz1) // We want to process the smaller part first to ensure log(n) stack size.
		{
			if (sz1)
			{
				++top;
				coASSERT(top < maxDepth);
				leftStack[top] = pl1;
				rightStack[top] = pr1;
			}

			if (sz0)
			{
				++top;
				coASSERT(top < maxDepth);
				leftStack[top] = pl0;
				rightStack[top] = pr0;
			}
		}
		else
		{
			if (sz0)
			{
				++top;
				coASSERT(top < maxDepth);
				leftStack[top] = pl0;
				rightStack[top] = pr0;
			}
			if (sz1)
			{
				++top;
				coASSERT(top < maxDepth);
				leftStack[top] = pl1;
				rightStack[top] = pr1;
			}
		}
	}
	// 
	// 	if (_l < _r)
	// 	{
	// 		const coUint pivot = Partition(_l, _r);
	// 		QuickSort(_l, pivot);
	// 		QuickSort(pivot + 1, _r);
	// 	}


	// QuickSort
	// Public-domain C implementation by Darel Rex Finley.
	// Pros:
	// - Not recursive
	// - Stack is small
	// - Less swaps, less moves
	// Cons: 
	// - More comparisons
	/*static const coUint maxLevels = 64;
	coUint beginStack[maxLevels];
	coUint endStack[maxLevels];
	beginStack[0] = 0;
	endStack[0] = _this.count;
	T* data = _this.data;
	for (coUint i = 0; i >= 0;)
	{
		coUint l = beginStack[i];
		coUint r = endStack[i] - 1;
		if (l < r)
		{
			const T& pivot = data[l];
			while (l < r)
			{
				while (_cmp(pivot, data[r]) && l < r)
					--r;
				if (l < r)
				{
					data[l] = data[r];
					++l;
				}
				while (_cmp(data[l], pivot) && l < r)
					++l;
				if (l < r)
				{
					data[r] = data[l];
					--r;
				}
			}
			data[l] = pivot;
			beginStack[i + 1] = l + 1;
			endStack[i + 1] = endStack[i];
			endStack[i] = l;
			++i;
			if (endStack[i] - beginStack[i] > endStack[i - 1] - beginStack[i - 1])
			{
				coSwap(beginStack[i], beginStack[i - 1]);
				coSwap(endStack[i], endStack[i - 1]);
			}
		}
		else
		{
			--i;
		}
	}*/

	/*auto Partition = [&](coUint _l, coUint _r) -> coUint
	{
		const T& pivot = _this.data[_l];
		coUint i = _l - 1;
		coUint j = _r + 1;
		for (;;)
		{
			do
			{
				++i;
			} while (_cmp(_this.data[i], pivot));

			do
			{
				--j;
			} while (_cmp(pivot, _this.data[j]));

			if (i >= j)
				return j;

			coSwap(_this.data[i], _this.data[j]);
		}
	};

	coDynamicArray<coUint> stack;
	coResize(stack, _this.count);*/
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
