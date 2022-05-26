// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include <container/array/coDynamicArray_f.h>
#include "../../vector/coVec3_f.h"
#include "coMesh_f.h"

void coComputeTriangleNormals(const coArray<coVec3>& vertices, const coArray<coUint32>& indices, coDynamicArray<coVec3>& outNormals)
{
	const coUint32 nbTriangles = indices.count / 3;
	coClear(outNormals);
	coResize(outNormals, nbTriangles);
	coUint32 index = 0;
	for (coUint triangleIdx = 0; triangleIdx < nbTriangles; ++triangleIdx)
	{
		const coVec3& a = vertices[indices[index++]];
		const coVec3& b = vertices[indices[index++]];
		const coVec3& c = vertices[indices[index++]];
		outNormals[triangleIdx] = coNormalize(coCross(b - a, c - b));
	}
}
