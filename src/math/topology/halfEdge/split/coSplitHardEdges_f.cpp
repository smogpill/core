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

void coSplitHardEdges(coHalfEdgeMesh& mesh, const coArray<coVec3>& faceNormals, coFloat angle)
{
	const coFloatx4 cosAngle = coCos(angle);
	auto& edges = mesh.halfEdges;
	auto& vertices = mesh.vertices;
	const coUint nbEdges = edges.count;

	for (coHalfEdge& edge : edges)
		edge.done = false;

	coDynamicArray<coBool> doneVertices;
	coResize(doneVertices, vertices.count, false);

	auto IsSharp = [&](const coUint32 eIdx)
	{
		const coHalfEdge& edgeA = edges[eIdx];
		const coHalfEdge& edgeB = edges[edgeA.nextRadial];
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
					const coHalfEdge& prev = edges[itEdge.prev];
					if (prev.nextRadial == itEdge.prev)
					{
						startEdgeIdx = itEdgeIdx;
						break;
					}
					itEdgeIdx = prev.nextRadial;
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
					itEdgeIdx = prev.nextRadial;
				} while (itEdgeIdx != edgeIdx);
			}

			// No border && no sharp edge -> ignore.
			if (startEdgeIdx == ~coUint32(0))
				continue;
		}

		// Rotate around the vertex from the first edge
		{
			coUint32 curVertexIdx = vertexIdx;
			coUint32 itEdgeIdx = startEdgeIdx;
			do
			{
				coHalfEdge& itEdge = edges[itEdgeIdx];

				// Sharp?
				if (IsSharp(itEdgeIdx))
				{
					// Create a new vertex
					const coVec3 pos = vertices[curVertexIdx];
					curVertexIdx = vertices.count;
					coPushBack(vertices, pos);
					edges[itEdge.nextRadial].nextRadial = itEdge.nextRadial;
					edges[itEdge.nextRadial].prevRadial = itEdge.nextRadial;
					itEdge.nextRadial = itEdgeIdx;
					itEdge.prevRadial = itEdgeIdx;
				}

				// Replace vertex in edge
				edges[itEdgeIdx].vertexIdx = curVertexIdx;
				itEdge.done = true;

				const coUint32 prevIdx = itEdge.prev;
				const coHalfEdge& prev = edges[prevIdx];
				if (prev.nextRadial == prevIdx)
					break;

				itEdgeIdx = prev.nextRadial;
				
			} while (itEdgeIdx != startEdgeIdx);
		}
	}
}

/*
	coDynamicArray<Change> changes;
	coReserve(changes, 1024);

	for (coUint vertIndex = 0; vertIndex < nbInitialVertices; ++vertIndex)
	{
		// Find first edge
		FaceEdge firstEdge = emptyEdge;
		{
			// Find border
			{
				VertexToFaceEdge* itFaceEdge = &vertexToFaceEdges[vertIndex];
				while (itFaceEdge->faceEdge != coUint32(-1))
				{
					const FaceEdge& edge = faceNeighbors[itFaceEdge->faceEdge];
					if (IsBorder(edge))
					{
						firstEdge = edge;
						break;
					}
					if (itFaceEdge->next == coUint32(-1))
						break;
					itFaceEdge = &vertexToFaceEdges[itFaceEdge->next];
				}
			}

			// No border found -> Find a sharp edge
			if (firstEdge == emptyEdge)
			{
				VertexToFaceEdge* itFaceEdge = &vertexToFaceEdges[vertIndex];
				while (itFaceEdge->faceEdge != coUint32(-1))
				{
					const FaceEdge& edge = faceNeighbors[itFaceEdge->faceEdge];
					if (IsSharp(edge))
					{
						firstEdge = edge;
						break;
					}
					if (itFaceEdge->next == coUint32(-1))
						break;
					itFaceEdge = &vertexToFaceEdges[itFaceEdge->next];
				}
			}
		}

		if (firstEdge == emptyEdge)
			continue;

		// TEMP TEMP TEMP TEMP
		if (nbInitialVertices == 88001 && nbTriangles == 174531 && vertIndex == 62709)
		{
			int x = 0; ++x;
		}
		// TEMP TEMP TEMP TEMP

		// Rotate around the vertex from the first edge
		coUint32 curVertex = vertIndex;
		FaceEdge edge = firstEdge;
		do
		{
			// Sharp?
			if (IsSharp(edge))
			{
				// Create a new vertex
				const coVec3 pos = positions[curVertex];
				curVertex = positions.count;
				coPushBack(positions) = pos;
			}

			// Replace vertex in face
			const coUint ind = edge.adjacentFace * 3 + ((edge.edgeInAdjacentFace + 1) % 3);
			if (indices[ind] != curVertex)
			{
				Change change;
				change.index = ind;
				change.vertex = curVertex;
				coPushBack(changes, change);
			}

			// Rotate to the next edge
			edge = GetNextEdge(vertIndex, edge);

			if (edge == emptyEdge)
			{
				break;
			}
		} while (edge != firstEdge);
	}

	for (const Change change : changes)
	{
		indices[change.index] = change.vertex;
	}*/