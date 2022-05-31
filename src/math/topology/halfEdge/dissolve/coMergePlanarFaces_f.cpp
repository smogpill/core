// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "../../../vector/coVec3_f.h"
#include "../coHalfEdgeMesh.h"
#include "coAbsorbFace_f.h"
#include "coMergePlanarFaces_f.h"

void coMergePlanarFaces(coHalfEdgeMesh& mesh, const coArray<coVec3>& faceNormals, coFloat tolerance)
{
	auto& edges = mesh.halfEdges;
	for (coUint32 edgeAIdx = 0; edgeAIdx < edges.count; ++edgeAIdx)
	{
		coHalfEdge& edgeA = edges[edgeAIdx];
		const coUint32 edgeBIdx = edgeA.nextRadial;
		if (edgeBIdx == edgeAIdx)
			continue;

		coHalfEdge& edgeB = edges[edgeBIdx];
		const coVec3& normalA = faceNormals[edgeA.faceIdx];
		const coVec3& normalB = faceNormals[edgeB.faceIdx];
		if (coAbs(coDot(normalA, normalB) - 1.0f) < tolerance)
		{
			coAbsorbFace(mesh, edgeAIdx, edgeB.faceIdx);
		}
	}
}
