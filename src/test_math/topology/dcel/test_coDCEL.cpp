// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_math/pch.h"
#include "test/unit/coTest.h"
#include <container/array/coDynamicArray_f.h>
#include "math/topology/dcel/coDCEL.h"

coTEST(coDCEL, constructor_0)
{
	coDCEL m({});
	coEXPECT(m.halfEdges.count == 0);
}

coTEST(coDCEL, constructor_1)
{
	coDCEL m(coArray<coUint32>({ 0, 1, 2 }));
	coEXPECT(m.halfEdges.count == 3);
	for (coUint i = 0; i < 3; ++i)
	{
		coHalfEdge& e = m.halfEdges[i];
		coEXPECT(e.faceIdx == 0);
		coEXPECT(e.twin == coUint32(-1));
		coEXPECT(e.next == (i + 1) % 3);
		coEXPECT(e.prev == (i + 2) % 3);
		coEXPECT(e.vertexIdx == i);
	}
}

coTEST(coDCEL, constructor_2)
{
	const coDynamicArray<coUint32> indices({ 0, 1, 2, 2, 1, 3 });
	coDCEL m(indices);
	coEXPECT(m.halfEdges.count == 6);

	for (coUint faceIdx = 0; faceIdx < 2; ++faceIdx)
	{
		for (coUint i = 0; i < 3; ++i)
		{
			coHalfEdge& e = m.halfEdges[faceIdx * 3 + i];
			coEXPECT(e.faceIdx == faceIdx);
			coEXPECT(e.next == faceIdx * 3 + (i + 1) % 3);
			coEXPECT(e.prev == faceIdx * 3 + (i + 2) % 3);
			coEXPECT(e.vertexIdx == indices[faceIdx * 3 + i]);
		}
	}
	const coUint32 expectedTwins[] = { coUint32(-1), 3, coUint32(-1), 1, coUint32(-1), coUint32(-1) };
	for (coUint32 i = 0; i < 6; ++i)
	{
		coEXPECT(m.halfEdges[i].twin == expectedTwins[i]);
	}
}

coTEST(coDCEL, constructor_2_2)
{
	const coDynamicArray<coUint32> indices({ 0, 1, 2, 3, 4, 5 });
	coDCEL m(indices);
	coEXPECT(m.halfEdges.count == 6);

	for (coUint faceIdx = 0; faceIdx < 2; ++faceIdx)
	{
		for (coUint i = 0; i < 3; ++i)
		{
			coHalfEdge& e = m.halfEdges[faceIdx * 3 + i];
			coEXPECT(e.faceIdx == faceIdx);
			coEXPECT(e.next == faceIdx * 3 + (i + 1) % 3);
			coEXPECT(e.prev == faceIdx * 3 + (i + 2) % 3);
			coEXPECT(e.vertexIdx == indices[faceIdx * 3 + i]);
		}
	}

	for (coUint32 i = 0; i < 6; ++i)
	{
		coEXPECT(m.halfEdges[i].twin == coUint32(-1));
	}
}
