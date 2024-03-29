// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coMergePlanarFaces_f.h"
#include "coAbsorbFace_f.h"
#include "../coDCEL.h"
#include "../../../vector/coVec3_f.h"
#include <debug/profiler/coProfile.h>

void coMergePlanarFaces(coDCEL& dcel, coFloat tolerance)
{
	coPROFILE_EVENT();

	auto& edges = dcel.halfEdges;
	const auto& faceNormals = dcel.faceNormals;
	for (coUint32 edgeAIdx = 0; edgeAIdx < edges.count; ++edgeAIdx)
	{
		coHalfEdge& edgeA = edges[edgeAIdx];
		if (!edgeA.HasTwin() || edgeA.IsDegenerate())
			continue;
		coHalfEdge& edgeB = edges[edgeA.twin];
		if (edgeB.IsDegenerate())
			continue;

		const coVec3& normalA = faceNormals[edgeA.faceIdx];
		const coVec3& normalB = faceNormals[edgeB.faceIdx];
		if (coAbs(coDot(normalA, normalB) - 1.0f) < tolerance)
		{
			coAbsorbTwinFace(dcel, edgeAIdx);
		}
	}
}
