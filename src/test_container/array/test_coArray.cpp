// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_container/pch.h"
#include "test/unit/coTest.h"
#include "container/array/coDynamicArray.h"
#include "container/array/coDynamicArray_f.h"
#include "container/array/coArray_f.h"
#include "lang/reflect/coNumericLimits.h"

coTEST(coArray, sort)
{
	{
		coDynamicArray<coInt> a;
		coSort<coInt>(a);
		coEXPECT(coIsSorted(a));
	}

	{
		coDynamicArray<coInt> a({ 3, 1, 2 });
		coSort<coInt>(a);
		coEXPECT(coIsSorted(a));
	}

	{
		coDynamicArray<coInt> a({ 1, 2, 3 });
		coSort<coInt>(a);
		coEXPECT(coIsSorted(a));
	}

	{
		coDynamicArray<coInt> a({ 1, 1, 1 });
		coSort<coInt>(a);
		coEXPECT(coIsSorted(a));
	}

	/*{
		coDynamicArray<coInt> a;
		coResize(a, coNumericLimits<decltype(a.count)>::Max());
		coSort<coInt>(a);
		coEXPECT(coIsSorted(a));
	}*/
}
