// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coSplitHardEdges_f.h"
#include "../visit/coVisit_f.h"
#include "../../../scalar/coFloat_f.h"
#include "../../../vector/coFloatx4_f.h"
#include "../../../vector/coVec3_f.h"

void coComputeFaceNormals(const coHalfEdgeMesh& mesh, coDynamicArray<coVec3>& outNormals)
{
	const auto& edges = mesh.halfEdges;
	const auto& vertices = mesh.vertices;

	coInt32 maxFaceIndex = -1;
	for (const coHalfEdge& edge : edges)
	{
		if (coInt32(edge.faceIdx) > maxFaceIndex)
			maxFaceIndex = edge.faceIdx;
	}

	coDynamicArray<coVec3> faceNormals;
	coResize(faceNormals, maxFaceIndex+1);

	for (const coHalfEdge& edge : edges)
		edge.done = false;

	for (coUint32 edgeIdx = 0; edgeIdx < edges.count; ++edgeIdx)
	{
		const coHalfEdge& edge = edges[edgeIdx];
		if (edge.done)
			continue;

		coVec3 normalSum = coVec3(0.0f);
		coUint32 itEdgeIdx = edgeIdx;
		do
		{
			const coHalfEdge& itEdge = edges[itEdgeIdx];
			itEdge.done = true;
			const coHalfEdge& next = edges[itEdge.next];
			const coVec3& itVert = vertices[itEdge.vertexIdx];
			const coVec3& nextVert = vertices[next.vertexIdx];

			// Using generalization: https://iquilezles.org/articles/areas/
			normalSum += coCross(nextVert, itVert);

			itEdgeIdx = itEdge.next;
		} while (itEdgeIdx != edgeIdx);

		faceNormals[edge.faceIdx] = coNormalize(normalSum);
	}
}

void coSplitHardEdges(coHalfEdgeMesh& mesh, coFloat angle)
{
	coDEBUG_CODE(mesh.CheckManifoldExceptHoles());
	const coFloatx4 cosAngle = coCos(angle);
	auto& edges = mesh.halfEdges;
	auto& vertices = mesh.vertices;
	const auto& faceNormals = mesh.faceNormals;
	const coUint nbEdges = edges.count;

	for (coHalfEdge& edge : edges)
		edge.done = false;

	coDynamicArray<coBool> doneVertices;
	coResize(doneVertices, vertices.count, false);

	auto IsBorder = [&](const coUint32 eIdx)
	{
		const coHalfEdge& edge = edges[eIdx];
		return edge.twin == eIdx;
	};

	auto IsSharp = [&](const coUint32 eIdx)
	{
		const coHalfEdge& edgeA = edges[eIdx];
		const coHalfEdge& edgeB = edges[edgeA.twin];
		const coVec3& normalA = faceNormals[edgeA.faceIdx];
		const coVec3& normalB = faceNormals[edgeB.faceIdx];
		return coAreAllTrue(coDot(normalA, normalB) <= cosAngle);
	};

	for (coUint32 edgeIdx = 0; edgeIdx < nbEdges; ++edgeIdx)
	{
		coHalfEdge& edge = edges[edgeIdx];
		if (edge.done || edge.IsDegenerate())
			continue;

		edge.done = true;

		const coUint32 vertexIdx = edge.vertexIdx;

		// Find first edge
		coUint32 startEdgeIdx = ~coUint32(0);
		{
			// Find border
			{
				coUint32 itEdgeIdx = edgeIdx;
				do
				{
					const coHalfEdge& itEdge = edges[itEdgeIdx];
					coASSERT(itEdge.vertexIdx == vertexIdx);
					const coUint32 twinIdx = itEdge.twin;
					if (twinIdx == itEdgeIdx)
					{
						startEdgeIdx = itEdgeIdx;
						break;
					}
						
					const coHalfEdge& twin = edges[twinIdx];
					itEdgeIdx = twin.next;
				} while (itEdgeIdx != edgeIdx);
			}

			// No border found -> Find a sharp edge
			if (startEdgeIdx == ~coUint32(0))
			{
				coUint32 itEdgeIdx = edgeIdx;
				do
				{
					const coHalfEdge& itEdge = edges[itEdgeIdx];
					coASSERT(itEdge.vertexIdx == vertexIdx);
					if (IsSharp(itEdgeIdx))
					{
						startEdgeIdx = itEdgeIdx;
						break;
					}
					const coHalfEdge& prev = edges[itEdge.prev];
					itEdgeIdx = prev.twin;
				} while (itEdgeIdx != edgeIdx);
			}

			// No border && no sharp edge -> ignore.
			if (startEdgeIdx == ~coUint32(0))
				continue;
		}

		// Rotate around the vertex with the following edges
		{
			coUint32 curVertexIdx = vertexIdx;
			coUint32 itEdgeIdx = startEdgeIdx;
			do
			{
				coHalfEdge& itEdge = edges[itEdgeIdx];

				// Sharp?
				if (!IsBorder(itEdgeIdx) && IsSharp(itEdgeIdx))
				{
					// Create a new vertex
					{
						const coVec3 pos = vertices[curVertexIdx];
						curVertexIdx = vertices.count;
						coPushBack(vertices, pos);
					}

					// Separate the twin vertex if necessary to avoid forming a non-manifold twin vertex
					// (having two face fans instead of one)
					if (true)
					{
						// Circle around the other vertex to find it there is a full circle (meaning the vertex is 100% surrounded by a face fan)
						const coUint32 itEdgeTwinIdx = itEdge.twin;
						coASSERT(itEdgeTwinIdx != itEdgeIdx);
						coUint32 itEdge2Idx = itEdgeTwinIdx;
						do
						{
							const coHalfEdge& itEdge2 = edges[itEdge2Idx];
							const coUint32 prev2Idx = itEdge2.prev;
							const coHalfEdge& prev2 = edges[prev2Idx];
							if (prev2.twin == prev2Idx)
							{
								// Create a new twin vertex
								const coUint32 vertex2Idx = vertices.count;
								{
									const coHalfEdge& itEdgeTwin = edges[itEdgeTwinIdx];
									const coVec3 pos = vertices[itEdgeTwin.vertexIdx];
									coPushBack(vertices, pos);
								}

								// Change the vertex of half of the circle
								coUint32 itEdge3Idx = itEdgeTwinIdx;
								for (;;)
								{
									coHalfEdge& itEdge3 = edges[itEdge3Idx];
									itEdge3.vertexIdx = vertex2Idx;
									const coUint32 prev3Idx = itEdge3.prev;
									const coHalfEdge& prev3 = edges[prev3Idx];
									if (prev3.twin == prev3Idx)
										break;
									itEdge3Idx = prev3.twin;
								}
								break;
							}

							itEdge2Idx = prev2.twin;
						} while (itEdge2Idx != itEdgeTwinIdx);
					}

					// Separate the half edges
					edges[itEdge.twin].twin = itEdge.twin;
					itEdge.twin = itEdgeIdx;
				}

				// Replace vertex in edge
				edges[itEdgeIdx].vertexIdx = curVertexIdx;
				itEdge.done = true;

				const coUint32 prevIdx = itEdge.prev;
				const coHalfEdge& prev = edges[prevIdx];
				if (prev.twin == prevIdx)
					break;

				itEdgeIdx = prev.twin;
				
			} while (itEdgeIdx != startEdgeIdx);
		}

		coDEBUG_CODE(mesh.CheckManifoldExceptHoles());
	}
	coDEBUG_CODE(mesh.CheckManifoldExceptHoles());
}
