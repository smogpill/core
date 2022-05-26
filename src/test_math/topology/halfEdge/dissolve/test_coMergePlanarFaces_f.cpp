// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_math/pch.h"
#include "test/unit/coTest.h"
#include <container/array/coDynamicArray_f.h>
#include <math/vector/coVec3_f.h>
#include "math/topology/mesh/coMesh_f.h"
#include "math/topology/halfEdge/coHalfEdgeMesh.h"
#include "math/topology/halfEdge/dissolve/coMergePlanarFaces_f.h"
#include "math/topology/halfEdge/dissolve/coDissolveDegenerateFaces_f.h"

coTEST(coMergePlanarFaces, simple)
{
	const coDynamicArray<coVec3> positions
	({
		coVec3(0, 0, 0),
		coVec3(1, 0, 0),
		coVec3(0, 1, 0),
		coVec3(1, 1, 0)
	});
	const coDynamicArray<coUint32> indices({ 0, 1, 2, 2, 1, 3 });
	coHalfEdgeMesh m(indices);
	coDynamicArray<coVec3> normals;
	coComputeTriangleNormals(positions, indices, normals);
	coMergePlanarFaces(m, normals, 1e-4f);
	coEXPECT(m.GetNbNonDegenerateFaces() == 1);
}

coTEST(coMergePlanarFaces, same_plane_but_not_connected)
{
	const coDynamicArray<coVec3> positions
	({
		coVec3(0, 0, 0),
		coVec3(1, 0, 0),
		coVec3(0, 1, 0),
		coVec3(1, 0, 0),
		coVec3(0, 1, 0),
		coVec3(1, 1, 0)
		});
	const coDynamicArray<coUint32> indices({ 0, 1, 2, 4, 3, 5 });
	coHalfEdgeMesh m(indices);
	coDynamicArray<coVec3> normals;
	coComputeTriangleNormals(positions, indices, normals);
	coMergePlanarFaces(m, normals, 1e-4f);
	coEXPECT(m.GetNbNonDegenerateFaces() == 2);
}
