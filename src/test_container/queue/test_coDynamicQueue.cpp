// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_container/pch.h"
#include "test/unit/coTest.h"
#include "container/queue/coDynamicQueue_f.h"
#include "lang/reflect/coNumericLimits.h"
#include "debug/log/coLog.h"

coTEST(coDynamicQueue, defaultValues)
{
	coDynamicQueue<coInt> a;
	coEXPECT(a.count == 0);
	coEXPECT(coIsEmpty(a));
}

coTEST(coDynamicQueue, Reserve)
{
	coDynamicQueue<coInt> a;
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
	coEXPECT(coIsEmpty(a));
}

coTEST(coDynamicQueue, Simple)
{
	coDynamicQueue<coInt> a;
	coEXPECT(coIsEmpty(a));
	coPushBack(a, 1);
	coEXPECT(!coIsEmpty(a));
	coEXPECT(coPopFront(a) == 1);
	coEXPECT(coIsEmpty(a));

	coPushBack(a, 2);
	coEXPECT(!coIsEmpty(a));
	coPushBack(a, 3);
	coEXPECT(!coIsEmpty(a));
	coEXPECT(coPopFront(a) == 2);
	coEXPECT(!coIsEmpty(a));
	coEXPECT(coPopFront(a) == 3);
	coEXPECT(coIsEmpty(a));
}

coTEST(coDynamicQueue, SimpleLoop)
{
	coDynamicQueue<coUint> a;
	for (coUint i = 0; i < 1000; ++i)
	{
		coEXPECT(coIsEmpty(a));
		coPushBack(a, i);
		coEXPECT(!coIsEmpty(a));
		coEXPECT(coPopFront(a) == i);
		coEXPECT(coIsEmpty(a));
	}
}

coTEST(coDynamicQueue, SequentialLoop)
{
	coDynamicQueue<coUint> a;
	for (coUint i = 0; i < 1000; ++i)
	{
		coPushBack(a, i);
		coEXPECT(!coIsEmpty(a));
		coEXPECT(a.count == i + 1);
	}
	coEXPECT(a.count == 1000);
	for (coUint i = 0; i < 1000; ++i)
	{
		coEXPECT(coPopFront(a) == i);
	}
	coEXPECT(coIsEmpty(a));
}
