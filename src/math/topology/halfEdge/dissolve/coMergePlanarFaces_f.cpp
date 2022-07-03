// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coMergePlanarFaces_f.h"
#include "coAbsorbFace_f.h"
#include "../coHalfEdgeMesh.h"
#include "../../../vector/coVec3_f.h"
#include <debug/profiler/coProfile.h>

void coMergePlanarFaces(coHalfEdgeMesh& mesh, coFloat tolerance)
{
	coPROFILE_EVENT();

	auto& edges = mesh.halfEdges;
	const auto& faceNormals = mesh.faceNormals;
	for (coUint32 edgeAIdx = 0; edgeAIdx < edges.count; ++edgeAIdx)
	{
		coHalfEdge& edgeA = edges[edgeAIdx];
		const coUint32 edgeBIdx = edgeA.twin;
		if (edgeBIdx == edgeAIdx)
			continue;

		if (edgeA.IsDegenerate())
			continue;
		coHalfEdge& edgeB = edges[edgeBIdx];
		if (edgeB.IsDegenerate())
			continue;

		const coVec3& normalA = faceNormals[edgeA.faceIdx];
		const coVec3& normalB = faceNormals[edgeB.faceIdx];
		if (coAbs(coDot(normalA, normalB) - 1.0f) < tolerance)
		{
			coAbsorbNextRadialFace(mesh, edgeAIdx);
		}
	}
}
