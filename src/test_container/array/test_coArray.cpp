// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_container/pch.h"
#include "test/unit/coTest.h"
#include "container/array/coDynamicArray.h"
#include "container/array/coDynamicArray_f.h"
#include "container/array/coArray_f.h"
#include "container/string/coDynamicString.h"
#include "container/string/coConstString_f.h"
#include "lang/reflect/coNumericLimits.h"

coTEST(coArray, sort)
{
	{
		coDynamicArray<coInt> a;
		coSort<coInt>(a);
		coEXPECT(coIsSorted(a));
	}

	{
		coDynamicArray<coInt> a{ 3, 1, 2 };
		coSort<coInt>(a);
		coEXPECT(coIsSorted(a));
	}

	{
		coDynamicArray<coInt> a{ 1, 2, 3 };
		coSort<coInt>(a);
		coEXPECT(coIsSorted(a));
	}

	{
		coDynamicArray<coInt> a{ 1, 1, 1 };
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

// coTEST(coArray, assignment)
// {
// 	{
// 		coArray<coUint> a;
// 		a = {};
// 		coEXPECT(a.count == 0);
// 
// 		a = { 0, 1, 2 };
// 		coEXPECT(a.count == 3);
// 		for (coUint i = 0; i < a.count; ++i)
// 			coEXPECT(a[i] == i);
// 	}
// 	
// 	{
// 		coArray<coConstString> a;
// 		a = { "0", "1", "2" };
// 		coEXPECT(a.count == 3);
// 		coEXPECT(a[0] == "0");
// 		coEXPECT(a[1] == "1");
// 		coEXPECT(a[2] == "2");
// 	}
// 
// 	{
// 		coDynamicString ds0("0");
// 		coDynamicString ds1("1");
// 		coDynamicString ds2("2");
// 		coConstString s0 = ds0;
// 		coConstString s1 = ds1;
// 		coConstString s2 = ds2;
// 		coArray<coConstString> a;
// 		a = { s0, s1, s2 };
// 		coEXPECT(a.count == 3);
// 		coEXPECT(a[0] == "0");
// 		coEXPECT(a[1] == "1");
// 		coEXPECT(a[2] == "2");
// 	}
// }
