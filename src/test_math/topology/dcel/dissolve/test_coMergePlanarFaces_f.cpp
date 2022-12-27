// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_math/pch.h"
#include "test/unit/coTest.h"
#include <container/array/coDynamicArray_f.h>
#include <math/vector/coVec3_f.h>
#include "math/topology/mesh/coMesh_f.h"
#include "math/topology/dcel/coDCEL.h"
#include "math/topology/dcel/dissolve/coMergePlanarFaces_f.h"
#include "math/topology/dcel/dissolve/coDissolveDegenerateFaces_f.h"

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
	coDCEL m(indices);
	coDynamicArray<coVec3> normals;
	coComputeTriangleNormals(positions, indices, normals);
	m.faceNormals = normals;
	coMergePlanarFaces(m, 1e-4f);
	coRemoveDegenerateFaces(m);
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
	coDCEL m(indices);
	coDynamicArray<coVec3> normals;
	coComputeTriangleNormals(positions, indices, normals);
	m.faceNormals = normals;
	coMergePlanarFaces(m, 1e-4f);
	coRemoveDegenerateFaces(m);
	coEXPECT(m.GetNbNonDegenerateFaces() == 2);
}

coTEST(coMergePlanarFaces, two_triangles_but_with_flat_vertex)
{
	const coDynamicArray<coVec3> positions
	({
		coVec3(0, 0, 0),
		coVec3(1, 0, 0),
		coVec3(0, 1, 0),
		coVec3(1, 1, 0),
		coVec3(0.5f, 0.5f, 0)
		});
	const coDynamicArray<coUint32> indices({ 0, 1, 4, 0, 4, 2, 4, 1, 3, 4, 3, 2 });
	coDCEL m(indices);
	coDynamicArray<coVec3> normals;
	coComputeTriangleNormals(positions, indices, normals);
	m.faceNormals = normals;
	coMergePlanarFaces(m, 1e-4f);
	coRemoveDegenerateFaces(m);
	coEXPECT(m.GetNbNonDegenerateFaces() == 1);
}

coTEST(coMergePlanarFaces, triangle_hole_in_larger_triangle)
{
	// 5
	//
	//   2
	//   0 1
	// 3          4
	const coDynamicArray<coVec3> positions
	({
		coVec3(1, 1, 0),
		coVec3(2, 1, 0),
		coVec3(1, 2, 0),
		coVec3(0, 0, 0),
		coVec3(5, 0, 0),
		coVec3(0, 5, 0)
		});
	const coDynamicArray<coUint32> indices({ 3, 4, 0, 0, 4, 1, 1, 4, 5, 2, 1, 5, 3, 2, 5, 3, 0, 2 });
	coDCEL m(indices);
	coDynamicArray<coVec3> normals;
	coComputeTriangleNormals(positions, indices, normals);
	m.faceNormals = normals;
	coMergePlanarFaces(m, 1e-4f);
	coRemoveDegenerateFaces(m);
	coEXPECT(m.GetNbNonDegenerateFaces() == 1);
}