// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_math/pch.h"
#include "test/unit/coTest.h"
#include <container/array/coDynamicArray_f.h>
#include "math/topology/halfEdge/coHalfEdgeMesh.h"
#include "math/topology/halfEdge/dissolve/coAbsorbFace_f.h"

coTEST(coAbsorbFace, one_triangle)
{
	coHalfEdgeMesh m(coArray<coUint32>({ 0, 1, 2 }));
	coEXPECT(!coAbsorbFace(m, 0, 1));
	coEXPECT(m.GetNbNonDegenerateFaces() == 1);
}

coTEST(coAbsorbFace, two_triangles_start_with_free_edge)
{
	coHalfEdgeMesh m(coArray<coUint32>({0, 1, 2, 2, 1, 3}));
	coEXPECT(coAbsorbFace(m, 0, 1));
	coEXPECT(m.GetNbNonDegenerateFaces() == 1);
}

coTEST(coAbsorbFace, two_triangles_start_with_connected_edge)
{
	coHalfEdgeMesh m(coArray<coUint32>({ 0, 1, 2, 2, 1, 3 }));
	coEXPECT(coAbsorbFace(m, 1, 1));
	coEXPECT(m.GetNbNonDegenerateFaces() == 1);
}

coTEST(coAbsorbFace, two_triangles_find_non_existing_face)
{
	coHalfEdgeMesh m(coArray<coUint32>({ 0, 1, 2, 2, 1, 3 }));
	coEXPECT(!coAbsorbFace(m, 0, 7));
	coEXPECT(m.GetNbNonDegenerateFaces() == 2);
}