// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coPolygon3_f.h"
#include "math/vector/coVec3_f.h"
#include <container/array/coDynamicArray_f.h>

void coTriangulateXY(const coPolygon3& poly, coDynamicArray<coUint32>& triangles, TriangulateScratch& scratch)
{
	coClear(scratch.remainingIndices);
	coClear(scratch.sorted);
	coClear(scratch.deviations);
	coClear(triangles);
	coASSERT(!coIsClockwiseXY(poly));

	const coUint32 nbPoints = poly.vertices.count;

	// Copy the provided points to a dynamic list to work on.
	{
		coResize(scratch.remainingIndices, nbPoints);
		for (coUint32 i = 0; i < nbPoints; ++i)
			scratch.remainingIndices[i] = i;
	}

	// Remove "ear" triangles one by one.
	coBool iterate = true;
	while (iterate)
	{
		iterate = false;

		// Modify the sort order
		{
			const coUint32 nbRemainingVertices = scratch.remainingIndices.count;
			coClear(scratch.sorted);
			coResize(scratch.sorted, nbRemainingVertices);
			for (coUint32 i = 0; i < nbRemainingVertices; ++i)
			{
				scratch.sorted[i] = i;
			}
			// Compute deviations
			{
				coClear(scratch.deviations);
				coResize(scratch.deviations, nbRemainingVertices);
				for (coUint32 vertIdx = 0; vertIdx < nbRemainingVertices; ++vertIdx)
				{
					const coUint32 prevIdx = vertIdx == 0 ? (nbRemainingVertices - 1) : (vertIdx - 1);
					const coUint32 nextIdx = vertIdx == (nbRemainingVertices - 1) ? 0 : (vertIdx + 1);
					const coVec3& prev = poly.vertices[scratch.remainingIndices[prevIdx]];
					const coVec3& cur = poly.vertices[scratch.remainingIndices[vertIdx]];
					const coVec3& next = poly.vertices[scratch.remainingIndices[nextIdx]];

					// Heuristic
					{
						// Should handle:
						// - differences between big and small triangles
						// - an outliner will be punished twice (one per edge), while its neighbor will be punished once (unlike the triangle normal heuristic).
						scratch.deviations[vertIdx] = coAbs(coSafeNormalize(prev - cur).z) + coAbs(coSafeNormalize(next - cur).z);

						// # Other attempts:

						// Using the Y delta directly. Does not differientate between very large triangles and small triangles.
						//aDeviations[uPoint] = MATH::Abs(vCur.GetY() - vPrev.GetY()) + MATH::Abs(vCur.GetY() - vNext.GetY());

						// Using the triangle normal. A neighbor of an outliner point can be affected too much but that outlier to the point 
						// where it can have a worse deviation because its triangle is smaller than the outlier's one. 
						//const VECTOR4 vNormal = VECTOR4::Normalize3(VECTOR4::CrossProduct(vCur - vPrev, vNext - vCur));
						//aDeviations[uPoint] = 1.0f - MATH::Abs(vNormal.GetY());
					}
				}
			}

			auto compare = [&](coUint32 a, coUint32 b)
			{
				return scratch.deviations[a] > scratch.deviations[b];
			};
			coSort(scratch.sorted, compare);
		}

		// For every vertex
		for (coUint32 sortedIdx = 0; sortedIdx < scratch.sorted.count; ++sortedIdx)
		{
			const coUint32 i = scratch.sorted[sortedIdx];
			const coUint32 remainingPrevIdx = i == 0 ? (scratch.remainingIndices.count - 1) : (i - 1);
			const coUint32 remainingCurIdx = i;
			const coUint32 remainingNextIdx = i == (scratch.remainingIndices.count - 1) ? 0 : (i + 1);
			const coUint32 prevIdx = scratch.remainingIndices[remainingPrevIdx];
			const coUint32 curIdx = scratch.remainingIndices[remainingCurIdx];
			const coUint32 nextIdx = scratch.remainingIndices[remainingNextIdx];
			const coVec3& prev = poly.vertices[prevIdx];
			const coVec3& cur = poly.vertices[curIdx];
			const coVec3& next = poly.vertices[nextIdx];

			// If interior vertex, continue
			{
				const coVec3 v1 = prev - cur;
				const coVec3 v2 = next - cur;
				const coVec3 cross = coCross(v1, v2);
				if (cross[1] <= 0.f)
				{
					continue;
				}
			}

			// If any other point is inside the triangle, continue
			{
				coBool ignore = false;
				for (coUint32 j = 0; j < scratch.remainingIndices.count; ++j)
				{
					// Skip the current triangle
					if (j == remainingPrevIdx || j == remainingCurIdx || j == remainingNextIdx)
						continue;

					const coUint32 n = scratch.remainingIndices[j];
					const coVec3& v = poly.vertices[n];
					if (coIsInsideTriangleXY(prev, cur, next, v))
					{
						ignore = true;
						break;
					}
				}
				if (ignore)
				{
					continue;
				}
			}

			iterate = true;

			// Create the triangle
			coPushBack(triangles, prevIdx);
			coPushBack(triangles, curIdx);
			coPushBack(triangles, nextIdx);

			// Remove vertex
			coRemoveOrderedByIndex(scratch.remainingIndices, i);
			break;
		}
	}
	coASSERT(triangles.count % 3 == 0);
}

coFloat coSignedAreaXY(const coPolygon3& poly)
{
	// Based on the shoelace formula: https://en.wikipedia.org/wiki/Shoelace_formula.

	coFloat sum = 0.0f;

	for (coUint32 curIdx = 0, nextIdx = 1; nextIdx < poly.vertices.count; ++curIdx, ++nextIdx)
	{
		const coVec3 cur = poly.vertices[curIdx];
		const coVec3 next = poly.vertices[nextIdx];
		sum += cur[0] * next[1] - next[0] * cur[1];
	}

	// And the last point with the first point
	if (poly.vertices.count)
	{
		const coVec3 cur = poly.vertices[poly.vertices.count - 1];
		const coVec3 next = poly.vertices[0];
		sum += cur[0] * next[1] - next[0] * cur[1];
	}

	return sum * 0.5f;
}
