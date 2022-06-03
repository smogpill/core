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
