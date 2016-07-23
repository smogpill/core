#include "test_container/pch.h"
#include "test/unit/coTest.h"
#include "container/array/coDynamicArray_f.h"
#include "lang/reflect/coNumericLimits.h"

coTEST(coDynamicArray, defaultValues)
{
	coDynamicArray<int> a;
	coEXPECT(a.allocator);
	coEXPECT(a.count == 0);
}

coTEST(coDynamicArray, Reserve)
{
	coDynamicArray<int> a;
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

coTEST(coDynamicArray, Resize)
{
	coDynamicArray<int> a;
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
