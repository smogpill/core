// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_math/pch.h"
#include "test/unit/coTest.h"
#include <container/array/coDynamicArray_f.h>
#include "math/topology/dcel/coDCEL.h"
#include "math/topology/dcel/dissolve/coCollapseEdges_f.h"

coTEST(coCollapseEdges, simple)
{
	const coDynamicArray<coVec3> positions
	({
		coVec3(0, 0, 0),
		coVec3(0, 0, 0),
		coVec3(0, 1, 0)
	});
	const coDynamicArray<coUint32> indices({ 0, 1, 2 });
	coDCEL m(indices, positions);
	coCollapseEdgesSmallerThanDist(m, 0.1f);
	coEXPECT(m.GetNbNonDegenerateFaces() == 0);
}

coTEST(coCollapseEdges, shared_neighbor_vertices)
{
	// In this case, collapse should not happen because it could create a non-manifold situation
	//      3
	//     /|\
	//    / | \
	//   /  2  \
	//  /  / \  \
	// / /     \ \
    // -----------
	// 0          1
	const coDynamicArray<coVec3> positions
	({
		coVec3(0, 0, 0),
		coVec3(0, 0, 0),
		coVec3(0, 1, 0),
		coVec3(0, 2, 0)
		});
	const coDynamicArray<coUint32> indices({ 0, 1, 2, 2, 1, 3, 0, 2, 3 });
	coDCEL m(indices, positions);
	coCollapseEdgesSmallerThanDist(m, 0.1f);
	coEXPECT(m.GetNbNonDegenerateFaces() == 3);
}
