// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include <test_container/pch.h>
#include <test/unit/coTest.h>
#include <container/map/coHashMap.h>
#include <container/map/coHashMap_f.h>
#include <math/hash/coHash_f.h>

coTEST(coHashMap, coGet)
{
	coHashMap<coUint64, coInt, 1024> h;

	coEXPECT(coGet(h, coFastHash64(7), 0) == 0);
	coEXPECT(coGet(h, coFastHash64(0), 0) == 0);

	coSet(h, coFastHash64(7), 777);
	coEXPECT(coGet(h, coFastHash64(7), 0) == 777);
	coSet(h, coFastHash64(8), 888);
	coEXPECT(coGet(h, coFastHash64(7), 0) == 777);
	coEXPECT(coGet(h, coFastHash64(8), 0) == 888);
	coSet(h, coFastHash64(7), 778);
	coEXPECT(coGet(h, coFastHash64(7), 0) == 778);
}

coTEST(coHashMap, coFind)
{
	coHashMap<coUint64, coInt, 1024> h;

	coEXPECT(coFind(h, coFastHash64(7)) == nullptr);

	coSet(h, coFastHash64(7), 777);
	coEXPECT(coFind(h, coFastHash64(7)) != nullptr);
	coEXPECT(coFind(h, coFastHash64(7))->value == 777);

	coSet(h, coFastHash64(8), 888);
	coEXPECT(coFind(h, coFastHash64(7)) != nullptr);
	coEXPECT(coFind(h, coFastHash64(8)) != nullptr);

	coEXPECT(coFind(h, coFastHash64(7))->value == 777);
	coEXPECT(coFind(h, coFastHash64(8))->value == 888);
	coEXPECT(coFind(h, coFastHash64(9)) == nullptr);
}