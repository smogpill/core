#include "test_container/pch.h"
#include "test/unit/coTest.h"
#include "container/array/coDynamicArray_f.h"

coTEST(coDynamicArray, reserve)
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
}
