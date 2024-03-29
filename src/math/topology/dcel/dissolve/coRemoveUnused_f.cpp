// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coRemoveUnused_f.h"
#include "../coDCEL.h"
#include <container/array/coDynamicArray_f.h>

void coRemoveUnusedVertices(coDCEL& dcel)
{
	coDynamicArray<coVec3> newVertices;
	const coArray<coVec3>& oldVertices = dcel.vertices;
	coDynamicArray<coUint32> oldVertexToNew;
	auto& halfEdges = dcel.halfEdges;
	coReserve(newVertices, oldVertices.count);
	coResize(oldVertexToNew, oldVertices.count, coUint32(-1));
	for (coUint32 halfEdgeIdx = 0; halfEdgeIdx < halfEdges.count; ++halfEdgeIdx)
	{
		coHalfEdge& halfEdge = halfEdges[halfEdgeIdx];
		if (halfEdge.next == halfEdgeIdx)
		{
			halfEdge.vertexIdx = coUint32(-1);
		}
		else
		{
			coUint32& newIdx = oldVertexToNew[halfEdge.vertexIdx];
			if (newIdx == coUint32(-1))
			{
				newIdx = newVertices.count;
				coPushBack(newVertices, oldVertices[halfEdge.vertexIdx]);
			}
			halfEdge.vertexIdx = newIdx;
		}
	}
	dcel.vertices = std::move(newVertices);
}

void coRemoveUnusedHalfEdges(coDCEL& dcel)
{
	auto& edges = dcel.halfEdges;
	coDynamicArray<coUint32> oldToNew;
	coDynamicArray<coHalfEdge> newEdges;
	coResize(oldToNew, edges.count);
	coReserve(newEdges, edges.count);
	for (coUint32 edgeIdx = 0; edgeIdx < edges.count; ++edgeIdx)
	{
		const coHalfEdge& edge = edges[edgeIdx];
		coASSERT(edge.next != coUint32(-1));
		coASSERT(edge.prev != coUint32(-1));
		if (edge.next == edgeIdx)
		{
			oldToNew[edgeIdx] = coUint32(-1);
		}
		else
		{
			oldToNew[edgeIdx] = newEdges.count;
			coPushBack(newEdges, edge);
		}
	}
	for (coHalfEdge& edge : newEdges)
	{
		coASSERT(oldToNew[edge.next] != coUint32(-1));
		coASSERT(oldToNew[edge.prev] != coUint32(-1));
		edge.next = oldToNew[edge.next];
		edge.prev = oldToNew[edge.prev];
		if (edge.HasTwin())
			edge.twin = oldToNew[edge.twin];
	}
	edges = std::move(newEdges);
}

void coRemoveUnusedFaces(coDCEL& dcel)
{
	auto& edges = dcel.halfEdges;
	auto& faceNormals = dcel.faceNormals;

	coUint32 oldNbFaces = faceNormals.count;
	if (oldNbFaces == 0)
	{
		coUint32 maxFaceIdx = coUint32(-1);
		for (const coHalfEdge& edge : edges)
			if (edge.faceIdx > maxFaceIdx)
				maxFaceIdx = edge.faceIdx;
		oldNbFaces = maxFaceIdx + 1;
	}

	coDynamicArray<coUint32> oldToNew;
	coDynamicArray<coVec3> newFaceNormals;
	coUint32 nbFaces = 0;
	coResize(oldToNew, oldNbFaces, coUint32(-1));
	if (faceNormals.count)
		coReserve(newFaceNormals, oldNbFaces);
	for (coHalfEdge& edge : edges)
	{
		coUint32& newFaceIdx = oldToNew[edge.faceIdx];
		if (newFaceIdx == coUint32(-1))
		{
			newFaceIdx = nbFaces++;
			if (faceNormals.count)
				coPushBack(newFaceNormals, faceNormals[edge.faceIdx]);
		}
		edge.faceIdx = newFaceIdx;
	}
	faceNormals = std::move(newFaceNormals);
}

void coRemoveUnused(coDCEL& dcel)
{
	coRemoveUnusedHalfEdges(dcel);
	coRemoveUnusedFaces(dcel);
	coRemoveUnusedVertices(dcel);
}
