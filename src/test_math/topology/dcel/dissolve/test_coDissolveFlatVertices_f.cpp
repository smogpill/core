// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_math/pch.h"
#include "test/unit/coTest.h"
#include <math/topology/dcel/dissolve/coDissolveFlatVertices_f.h>
#include <math/topology/dcel/dissolve/coDissolveDegenerateFaces_f.h>
#include <math/topology/dcel/check/coCheckLinks_f.h>
#include <math/topology/dcel/coDCEL.h>
#include <container/array/coDynamicArray_f.h>

coTEST(coDissolveFlatVertices, empty)
{
	coDCEL dcel;
	coDissolveFlatVertices(dcel, 0.01f);
	coEXPECT(dcel.GetNbNonDegenerateFaces() == 0);
	coEXPECT(dcel.halfEdges.count == 0);
}

coTEST(coDissolveFlatVertices, triangle)
{
	coDynamicArray<coVec3> vertices({
		coVec3(0, 0, 0),
		coVec3(1, 0, 0),
		coVec3(1, 1, 0),
	});
	coDCEL dcel;
	dcel.AddFace(0, vertices);
	coDissolveFlatVertices(dcel, 0.01f);
	coEXPECT(dcel.GetNbNonDegenerateFaces() == 1);
	coEXPECT(dcel.GetNbAliveEdges() == 3);
	coCheckEdgesNotLinkedToDeadEdges(dcel);
}

coTEST(coDissolveFlatVertices, degenerateTriangle)
{
	coDynamicArray<coVec3> vertices({
		coVec3(0, 0, 0),
		coVec3(0.5f, 0, 0),
		coVec3(1, 0, 0),
		});
	coDCEL dcel;
	dcel.AddFace(0, vertices);
	coDissolveFlatVertices(dcel, 0.01f);
	coEXPECT(dcel.GetNbNonDegenerateFaces() == 0);
	coEXPECT(dcel.GetNbAliveEdges() == 0);
	coCheckEdgesNotLinkedToDeadEdges(dcel);
}

coTEST(coDissolveFlatVertices, simple)
{
	coDynamicArray<coVec3> vertices({
		coVec3(0, 0, 0),
		coVec3(0.5f, 0, 0),
		coVec3(1, 0, 0),
		coVec3(1, 1, 0),
		});
	coDCEL dcel;
	dcel.AddFace(0, vertices);
	coDissolveFlatVertices(dcel, 0.01f);
	coEXPECT(dcel.GetNbNonDegenerateFaces() == 1);
	coEXPECT(dcel.GetNbAliveEdges() == 3);
	coCheckEdgesNotLinkedToDeadEdges(dcel);
}

coTEST(coDissolveFlatVertices, Twin0)
{
	coDCEL dcel;
	dcel.vertices = 
	{
		coVec3(0, 0, 0),
		coVec3(0.5f, 0, 0),
		coVec3(1, 0, 0),
		coVec3(1, -1, 0),
		coVec3(0, 1, 0)
	};

	const coUint32 a0 = dcel.AddFace(0, coDynamicArray<coUint32>({ 0, 1, 2, 3 }));
	const coUint32 b0 = dcel.AddFace(1, coDynamicArray<coUint32>({ 1, 0, 4 }));
	dcel.halfEdges[a0].twin = b0;
	dcel.halfEdges[b0].twin = a0;
	coDissolveFlatVertices(dcel, 0.01f);
	coEXPECT(dcel.GetNbNonDegenerateFaces() == 2);
	coEXPECT(dcel.GetNbAliveEdges() == 7);
	coCheckEdgesNotLinkedToDeadEdges(dcel);
}

coTEST(coDissolveFlatVertices, Twin1)
{
	coDCEL dcel;
	dcel.vertices = 
	{
		coVec3(0, 0, 0),
		coVec3(0.5f, 0, 0),
		coVec3(1, 0, 0),
		coVec3(1, -1, 0),
		coVec3(0, 1, 0)
	};

	const coUint32 a0 = dcel.AddFace(0, coDynamicArray<coUint32>({ 0, 1, 2, 3 }));
	const coUint32 b0 = dcel.AddFace(1, coDynamicArray<coUint32>({ 2, 1, 4 }));
	dcel.halfEdges[a0+1].twin = b0;
	dcel.halfEdges[b0].twin = a0+1;
	coDissolveFlatVertices(dcel, 0.01f);
	coEXPECT(dcel.GetNbNonDegenerateFaces() == 2);
	coEXPECT(dcel.GetNbAliveEdges() == 7);
	coCheckEdgesNotLinkedToDeadEdges(dcel);
}

coTEST(coDissolveFlatVertices, twoSeparateTwinFaces)
{
	coDCEL dcel;
	dcel.vertices = coDynamicArray<coVec3>(
	{
		coVec3(0, 0, 0),
		coVec3(0.5f, 0, 0),
		coVec3(1, 0, 0),
		coVec3(1, -1, 0),
		coVec3(0, 1, 0),
		coVec3(1, 1, 0),
	});

	const coUint32 a0 = dcel.AddFace(0, coDynamicArray<coUint32>({ 0, 1, 2, 3 }));
	const coUint32 b0 = dcel.AddFace(1, coDynamicArray<coUint32>({ 1, 0, 4 }));
	const coUint32 c0 = dcel.AddFace(1, coDynamicArray<coUint32>({ 2, 1, 5 }));
	dcel.halfEdges[a0].twin = b0;
	dcel.halfEdges[b0].twin = a0;
	dcel.halfEdges[a0 + 1].twin = c0;
	dcel.halfEdges[c0].twin = a0 + 1;
	coDissolveFlatVertices(dcel, 0.01f);
	coEXPECT(dcel.GetNbNonDegenerateFaces() == 3);
	coEXPECT(dcel.GetNbAliveEdges() == 10);
	coCheckEdgesNotLinkedToDeadEdges(dcel);
}

coTEST(coDissolveFlatVertices, twoTwinFacesTwinAsWell)
{
	coDCEL dcel;
	dcel.vertices = 
	{
		coVec3(0, 0, 0),
		coVec3(0.5f, 0, 0),
		coVec3(1, 0, 0),
		coVec3(1, -1, 0),
		coVec3(0, 1, 0),
	};

	const coUint32 a0 = dcel.AddFace(0, coDynamicArray<coUint32>({ 0, 1, 2, 3 }));
	const coUint32 b0 = dcel.AddFace(1, coDynamicArray<coUint32>({ 1, 0, 4 }));
	const coUint32 c0 = dcel.AddFace(1, coDynamicArray<coUint32>({ 2, 1, 4 }));
	dcel.halfEdges[a0].twin = b0;
	dcel.halfEdges[b0].twin = a0;
	dcel.halfEdges[a0 + 1].twin = c0;
	dcel.halfEdges[c0].twin = a0 + 1;
	dcel.halfEdges[b0 + 2].twin = c0 + 1;
	dcel.halfEdges[c0 + 1].twin = b0 + 2;
	coDissolveFlatVertices(dcel, 0.01f);
	coEXPECT(dcel.GetNbNonDegenerateFaces() == 3);
	coEXPECT(dcel.GetNbAliveEdges() == 10);
	coCheckEdgesNotLinkedToDeadEdges(dcel);
}

coTEST(coDissolveFlatVertices, twinToDissolve)
{
	coDCEL dcel;
	dcel.vertices =
	{
		coVec3(0, 0, 0),
		coVec3(0.5f, 0, 0),
		coVec3(1, 0, 0),
		coVec3(1, -1, 0),
		coVec3(0, 1, 0),
	};

	const coUint32 a0 = dcel.AddFace(0, coDynamicArray<coUint32>({ 0, 1, 2, 3 }));
	const coUint32 b0 = dcel.AddFace(1, coDynamicArray<coUint32>({ 2, 1, 0, 4 }));
	dcel.halfEdges[a0].twin = b0 + 1;
	dcel.halfEdges[b0 + 1].twin = a0;
	dcel.halfEdges[a0 + 1].twin = b0;
	dcel.halfEdges[b0].twin = a0 + 1;
	coDissolveFlatVertices(dcel, 0.01f);
	coEXPECT(dcel.GetNbNonDegenerateFaces() == 2);
	coEXPECT(dcel.GetNbAliveEdges() == 6);
	coCheckEdgesNotLinkedToDeadEdges(dcel);
}

coTEST(coDissolveFlatVertices, twinBecomingDegenerate)
{
	coDCEL dcel;
	dcel.vertices =
	{
		coVec3(0, 0, 0),
		coVec3(0.5f, 0, 0),
		coVec3(1, 0, 0),
		coVec3(1, -1, 0),
	};

	const coUint32 a0 = dcel.AddFace(0, coDynamicArray<coUint32>({ 0, 1, 2, 3 }));
	const coUint32 b0 = dcel.AddFace(1, coDynamicArray<coUint32>({ 2, 1, 0 }));
	dcel.halfEdges[a0].twin = b0 + 1;
	dcel.halfEdges[b0 + 1].twin = a0;
	dcel.halfEdges[a0 + 1].twin = b0;
	dcel.halfEdges[b0].twin = a0 + 1;
	coDissolveFlatVertices(dcel, 0.01f);
	coEXPECT(dcel.GetNbNonDegenerateFaces() == 1);
	coEXPECT(dcel.GetNbAliveEdges() == 3);
	coCheckEdgesNotLinkedToDeadEdges(dcel);
}
