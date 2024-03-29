// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_math/pch.h"
#include "test/unit/coTest.h"
#include <container/array/coDynamicArray_f.h>
#include "math/topology/dcel/coDCEL.h"
#include "math/topology/dcel/dissolve/coAbsorbFace_f.h"

coTEST(coAbsorbTwinFace, one_triangle)
{
	coDCEL m(coArray<coUint32>({ 0, 1, 2 }));
	coEXPECT(coAbsorbTwinFace(m, 0) == coUint32(-1));
	coEXPECT(m.GetNbNonDegenerateFaces() == 1);
}

coTEST(coAbsorbTwinFace, two_triangles_start)
{
	coDCEL m(coArray<coUint32>({0, 1, 2, 2, 1, 3}));
	coEXPECT(coAbsorbTwinFace(m, 0) == coUint32(-1));
	coEXPECT(coAbsorbTwinFace(m, 2) == coUint32(-1));
	coEXPECT(coAbsorbTwinFace(m, 1) != coUint32(-1));
	coEXPECT(m.GetNbNonDegenerateFaces() == 1);
}

coTEST(coAbsorbTwinFace, third_triangle_in_middle_of_chain)
{
	coDCEL mesh;
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
		mesh.SetTwins(contact[0], contact[1]);

	// Temp
	{
		coDCEL m = mesh;
		coEXPECT(coAbsorbTwinFace(m, 11) != coUint32(-1));
	}

	for (const coUint32* contact : contacts)
		for (coUint32 i = 0; i < 2; ++i)
		{
			coDCEL m = mesh;
			coEXPECT(coAbsorbTwinFace(m, contact[i]) != coUint32(-1));
			m.Check();
			coEXPECT(m.GetNbNonDegenerateFaces() == 2);
		}
}

coTEST(coAbsorbTwinFace, all_edges_of_face_A_are_contacts)
{
	coDCEL mesh;
	const coUint32 firstEdgeA = mesh.AddFace(0, 3);
	const coUint32 firstEdgeB = mesh.AddFace(1, 6);
	const coUint32 contacts[][2] = 
	{ 
		{firstEdgeA + 0, firstEdgeB + 0}, 
		{firstEdgeA + 1, firstEdgeB + 1}, 
		{firstEdgeA + 2, firstEdgeB + 2} 
	};
	for (const coUint32* contact : contacts)
		mesh.SetTwins(contact[0], contact[1]);

	for (const coUint32* contact : contacts)
		for (coUint32 i = 0; i < 2; ++i)
		{
			coDCEL m = mesh;
			coEXPECT(coAbsorbTwinFace(m, contact[i]) != coUint32(-1));
			m.Check();
			coEXPECT(m.GetNbNonDegenerateFaces() == 1);
		}
}
