// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_container/pch.h"
#include "test/unit/coTest.h"
#include "container/array/coDynamicArray_f.h"
#include "lang/reflect/coNumericLimits.h"
#include "debug/log/coLog.h"

coTEST(coDynamicArray, defaultValues)
{
	coDynamicArray<coInt> a;
	coEXPECT(a.count == 0);
}

coTEST(coDynamicArray, Reserve)
{
	coDynamicArray<coInt> a;
	coEXPECT(a.capacity == 0);
	coReserve(a, 0);
	coEXPECT(a.capacity == 0);
	coReserve(a, 5);
	coEXPECT(a.capacity >= 5);
	coReserve(a, 3);
	coEXPECT(a.capacity >= 5);
	coReserve(a, 0);
	coEXPECT(a.capacity >= 5);
	coReserve(a, 7);
	coEXPECT(a.capacity >= 7);
	/*coReserve(a, coNumericLimits<decltype(a.capacity)>::Max());
	coEXPECT(a.capacity == coNumericLimits<decltype(a.capacity)>::Max());*/
}

coTEST(coDynamicArray, initializerList)
{
	{
		coDynamicArray<coUint> a({});
		coEXPECT(a.count == 0);
		coEXPECT(a.capacity == 0);
	}

	{
		coDynamicArray<coUint> a({0, 1, 2});
		coEXPECT(a.count == 3);
		coEXPECT(a.capacity >= 3);
		for (coUint i = 0; i < a.count; ++i)
		{
			coEXPECT(a[i] == i);
		}
	}
}

coTEST(coDynamicArray, Resize)
{
	// Check sizes
	{
		coDynamicArray<coInt> a;
		coEXPECT(a.count == 0);
		coResize(a, 0);
		coEXPECT(a.count == 0);
		coEXPECT(a.capacity == 0);
		coResize(a, 5);
		coEXPECT(a.count == 5);
		coEXPECT(a.capacity >= a.count);
		coResize(a, 3);
		coEXPECT(a.count == 3);
		coEXPECT(a.capacity >= a.count);
		coResize(a, 0);
		coEXPECT(a.count == 0);
		coEXPECT(a.capacity >= a.count);
		coResize(a, 7);
		coEXPECT(a.count == 7);
		coEXPECT(a.capacity >= a.count);
		/*coResize(a, coNumericLimits<decltype(a.count)>::Max());
		coEXPECT(a.count == coNumericLimits<decltype(a.count)>::Max());
		coEXPECT(a.capacity >= a.count);*/
	}

	// Check constructors
	{
		struct V
		{
			V()
			{
				static coUint v = 0;
				x = v;
				++v;
			}
			coUint x;
		};
		coDynamicArray<V> a;
		coResize(a, 3);
		coEXPECT(a.count == 3);
		for (coUint i = 0; i < a.count; ++i)
		{
			const V& v = a[i];
			coEXPECT(v.x == i);
		}
	}
}

coTEST(coDynamicArray, coInsert)
{
	{
		coDynamicArray<coInt> a{};
		coInsert(a, 0, 7);
		coEXPECT(a.count == 1);
		coEXPECT(a.capacity >= 1);
		coEXPECT(a[0] == 7);
	}
	{
		coDynamicArray<coInt> a{ 1, 2, 3 };
		coInsert(a, 0, 7);
		coEXPECT(a.count == 4);
		coEXPECT(a.capacity >= 4);
		coEXPECT(a[0] == 7);
		coEXPECT(a[1] == 1);
		coEXPECT(a[2] == 2);
		coEXPECT(a[3] == 3);
	}
	{
		coDynamicArray<coInt> a{ 1, 2, 3 };
		coInsert(a, 1, 7);
		coEXPECT(a.count == 4);
		coEXPECT(a.capacity >= 4);
		coEXPECT(a[0] == 1);
		coEXPECT(a[1] == 7);
		coEXPECT(a[2] == 2);
		coEXPECT(a[3] == 3);
	}
	{
		coDynamicArray<coInt> a{ 1, 2, 3 };
		coInsert(a, 2, 7);
		coEXPECT(a.count == 4);
		coEXPECT(a.capacity >= 4);
		coEXPECT(a[0] == 1);
		coEXPECT(a[1] == 2);
		coEXPECT(a[2] == 7);
		coEXPECT(a[3] == 3);
	}
	{
		coDynamicArray<coInt> a{ 1, 2, 3 };
		coInsert(a, 3, 7);
		coEXPECT(a.count == 4);
		coEXPECT(a.capacity >= 4);
		coEXPECT(a[0] == 1);
		coEXPECT(a[1] == 2);
		coEXPECT(a[2] == 3);
		coEXPECT(a[3] == 7);
	}
}
