// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_math/pch.h"
#include "test/unit/coTest.h"
#include <container/array/coDynamicArray_f.h>
#include "math/topology/halfEdge/coHalfEdgeMesh.h"
#include "math/topology/halfEdge/dissolve/coAbsorbFace_f.h"

coTEST(coAbsorbNextRadialFace, one_triangle)
{
	coHalfEdgeMesh m(coArray<coUint32>({ 0, 1, 2 }));
	coEXPECT(!coAbsorbNextRadialFace(m, 0));
	coEXPECT(m.GetNbNonDegenerateFaces() == 1);
}

coTEST(coAbsorbNextRadialFace, two_triangles_start)
{
	coHalfEdgeMesh m(coArray<coUint32>({0, 1, 2, 2, 1, 3}));
	coEXPECT(!coAbsorbNextRadialFace(m, 0));
	coEXPECT(!coAbsorbNextRadialFace(m, 2));
	coEXPECT(coAbsorbNextRadialFace(m, 1));
	coEXPECT(m.GetNbNonDegenerateFaces() == 1);
}

coTEST(coAbsorbNextRadialFace, third_triangle_in_middle_of_chain)
{
	coHalfEdgeMesh m;
	const coUint32 firstEdgeA = m.AddFace(0, 3);
	const coUint32 firstEdgeB = m.AddFace(1, 3);
	const coUint32 firstEdgeC = m.AddFace(2, 7);
	m.SetRadials(firstEdgeA, firstEdgeC + 6);
	m.SetRadials(firstEdgeA + 1, firstEdgeC + 2);
	m.SetRadials(firstEdgeB, firstEdgeC + 5);
	m.SetRadials(firstEdgeB + 1, firstEdgeC + 4);
	m.SetRadials(firstEdgeB + 2, firstEdgeC + 3);
	coEXPECT(coAbsorbNextRadialFace(m, 0));
	m.Check();
	coEXPECT(m.GetNbNonDegenerateFaces() == 2);
}
