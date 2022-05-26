// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_math/pch.h"
#include "test/unit/coTest.h"
#include <container/array/coDynamicArray_f.h>
#include "math/topology/halfEdge/coHalfEdgeMesh.h"

coTEST(coHalfEdgeMesh, constructor_0)
{
	coHalfEdgeMesh m({});
	coEXPECT(m.halfEdges.count == 0);
}

coTEST(coHalfEdgeMesh, constructor_1)
{
	coHalfEdgeMesh m(coArray<coUint32>({ 0, 1, 2 }));
	coEXPECT(m.halfEdges.count == 3);
	for (coUint i = 0; i < 3; ++i)
	{
		coHalfEdge& e = m.halfEdges[i];
		coEXPECT(e.faceIdx == 0);
		coEXPECT(e.nextRadial == i);
		coEXPECT(e.prevRadial == i);
		coEXPECT(e.next == (i + 1) % 3);
		coEXPECT(e.prev == (i + 2) % 3);
		coEXPECT(e.vertexIdx == i);
	}
}

coTEST(coHalfEdgeMesh, constructor_2)
{
	const coDynamicArray<coUint32> indices({ 0, 1, 2, 2, 1, 3 });
	coHalfEdgeMesh m(indices);
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
	const coUint32 expectedPrevRadials[] = { 0, 3, 2, 1, 4, 5 };
	const coUint32 expectedNextRadials[] = {0, 3, 2, 1, 4, 5};
	for (coUint32 i = 0; i < 6; ++i)
	{
		coEXPECT(m.halfEdges[i].prevRadial == expectedPrevRadials[i]);
		coEXPECT(m.halfEdges[i].nextRadial == expectedNextRadials[i]);
	}
}

coTEST(coHalfEdgeMesh, constructor_2_2)
{
	const coDynamicArray<coUint32> indices({ 0, 1, 2, 3, 4, 5 });
	coHalfEdgeMesh m(indices);
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
		coEXPECT(m.halfEdges[i].prevRadial == i);
		coEXPECT(m.halfEdges[i].nextRadial == i);
	}
}
