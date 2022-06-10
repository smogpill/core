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
	coHalfEdgeMesh mesh;
	const coUint32 firstEdgeA = mesh.AddFace(0, 3);
	const coUint32 firstEdgeB = mesh.AddFace(1, 3);
	const coUint32 firstEdgeC = mesh.AddFace(2, 7);
	const coUint32 contacts[][2] = 
	{
		{firstEdgeA, firstEdgeC + 6},
		{firstEdgeA + 1, firstEdgeC + 2},
		{firstEdgeB, firstEdgeC + 5},
		{firstEdgeB + 1, firstEdgeC + 4},
		{firstEdgeB + 2, firstEdgeC + 3}
	};
	for (const coUint32* contact : contacts)
		mesh.SetRadials(contact[0], contact[1]);

	for (const coUint32* contact : contacts)
		for (coUint32 i = 0; i < 2; ++i)
		{
			coHalfEdgeMesh m = mesh;
			coEXPECT(coAbsorbNextRadialFace(m, contact[i]));
			m.Check();
			coEXPECT(m.GetNbNonDegenerateFaces() == 2);
		}
}

coTEST(coAbsorbNextRadialFace, all_edges_of_face_A_are_contacts)
{
	coHalfEdgeMesh mesh;
	const coUint32 firstEdgeA = mesh.AddFace(0, 3);
	const coUint32 firstEdgeB = mesh.AddFace(1, 6);
	const coUint32 contacts[][2] = 
	{ 
		{firstEdgeA + 0, firstEdgeB + 0}, 
		{firstEdgeA + 1, firstEdgeB + 1}, 
		{firstEdgeA + 2, firstEdgeB + 2} 
	};
	for (const coUint32* contact : contacts)
		mesh.SetRadials(contact[0], contact[1]);

	for (const coUint32* contact : contacts)
		for (coUint32 i = 0; i < 2; ++i)
		{
			coHalfEdgeMesh m = mesh;
			coEXPECT(coAbsorbNextRadialFace(m, contact[i]));
			m.Check();
			coEXPECT(m.GetNbNonDegenerateFaces() == 1);
		}
}
