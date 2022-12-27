// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_math/pch.h"
#include "test/unit/coTest.h"
#include <math/topology/dcel/dissolve/coDissolveFlatVertices_f.h>
#include <math/topology/dcel/coDCEL.h>
#include <container/array/coDynamicArray_f.h>

coTEST(coDissolveFlatVertices, empty)
{
	coDCEL dcel;
	coDissolveFlatVertices(dcel, 0.01f);
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
	coEXPECT(dcel.GetNbAliveEdges() == 3);
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
	coEXPECT(dcel.GetNbAliveEdges() == 3);
}
