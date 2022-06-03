// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coPolygon3_f.h"
#include "math/vector/coVec3_f.h"
#include "math/shape/triangle/coTriangle_f.h"
#include <container/array/coDynamicArray_f.h>

void _coPrepareTriangulate(const coPolygon3& poly, coDynamicArray<coUint32>& triangles, coTriangulateScratch& scratch)
{
	coClear(scratch.remainingIndices);
	coClear(scratch.sorted);
	coClear(scratch.deviations);
	coClear(triangles);
	const coUint32 nbPoints = poly.vertices.count;

	// Copy the provided points to a dynamic list to work on.
	{
		coResize(scratch.remainingIndices, nbPoints);
		for (coUint32 i = 0; i < nbPoints; ++i)
			scratch.remainingIndices[i] = i;
	}
}

coFORCE_INLINE coBool _coIsCornerConvexXY(const coVec3& a, const coVec3& b, const coVec3& c)
{
	return !coAreAllFalse(coBroadcastZ(coCross(a - b, c - b)) >= coFloatx3(0.0f));
}
coFORCE_INLINE coBool _coIsInsideTriangleXY(const coVec3& a, const coVec3& b, const coVec3& c, const coVec3& p)
{
	const coBool b1 = _coIsCornerConvexXY(p, a, b);
	const coBool b2 = _coIsCornerConvexXY(p, b, c);
	const coBool b3 = _coIsCornerConvexXY(p, c, a);

	return (b1 == b2) & (b2 == b3);
}

void coTriangulateAssumingFlat(const coPolygon3& poly, coDynamicArray<coUint32>& triangleVertices, coTriangulateScratch& scratch, const coVec3& planeNormal)
{
	// TODO: Check Ear-clipping Based Algorithms of Generating High - quality Polygon Triangulation Gang Mei1, John C.Tipper1 and Nengxiong Xu

	//coASSERT(!coIsClockwiseXY(poly));
	//coASSERT(!coContainsFlatVertices(poly)); // flat vertex -> Degenerate polygon
	_coPrepareTriangulate(poly, triangleVertices, scratch);

	const coUint32 expectedNbTriangles = poly.vertices.count - 2;
	coUint32 nbRemainingTriangles = expectedNbTriangles;
	coUint32 nbRemainingIterations = expectedNbTriangles;

	// Remove "ear" triangles one by one.
	while (nbRemainingIterations)
	{
		// For every vertex
		for (coUint32 i = 0; i < scratch.remainingIndices.count; ++i)
		{
			const coUint32 remainingPrev = i == 0 ? (scratch.remainingIndices.count - 1) : (i - 1);
			const coUint32 remainingCur = i;
			const coUint32 remainingNext = i == (scratch.remainingIndices.count - 1) ? 0 : (i + 1);
			const coUint32 prevIdx = scratch.remainingIndices[remainingPrev];
			const coUint32 curIdx = scratch.remainingIndices[remainingCur];
			const coUint32 nextIdx = scratch.remainingIndices[remainingNext];
			const coVec3& prev = poly.vertices[prevIdx];
			const coVec3& cur = poly.vertices[curIdx];
			const coVec3& next = poly.vertices[nextIdx];

			// If interior vertex, continue
			{
				const coVec3 v1 = prev - cur;
				const coVec3 v2 = next - cur;
				if (coDot(coCross(v1, v2), planeNormal).x < 0.f)
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
					if (j == remainingPrev || j == remainingCur || j == remainingNext)
						continue;

					const coUint32 n = scratch.remainingIndices[j];
					const coVec3& v = poly.vertices[n];
					if (coOverlap(coTriangle(prev, cur, next), v))
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

			coPushBack(triangleVertices, prevIdx);
			coPushBack(triangleVertices, curIdx);
			coPushBack(triangleVertices, nextIdx);
			--nbRemainingTriangles;
			if (!nbRemainingTriangles)
			{
				coASSERT(triangleVertices.count % 3 == 0);
				coASSERT(triangleVertices.count / 3 == expectedNbTriangles);
				return;
			}

			// Remove vertex
			coRemoveOrderedByIndex(scratch.remainingIndices, i);
			--i;
		}
		--nbRemainingIterations;
	}

	// Some triangles can be dismissed since they can be flat/colinear.
	// Check the issue with this case:
	// [0]{ -17.7500000, -10.2500000, 6.42884827}
	// [1]{ -18.5347691, -10.8032837, 6.42884064 }
	// [2]{ -18.2500000, -9.75000000, 6.42884827 }
	// [3]{ -17.4375000, -9.18750000, 6.42885590 }
	// [4]{ -17.1250000, -9.37500000, 6.42885590 }
	// [5]{ -16.9375000, -9.31250000, 6.42885590 }
	// Can be viewed some plotting software, like desmos.com
	//
	// With the last triangle to be found being 2,4,5, and sadly being flat.
	//coASSERT(nbRemainingTriangles == 0); // The algorithm did not manage to make enough triangles.
}

void coTriangulateWithVaryingZ(const coPolygon3& poly, coDynamicArray<coUint32>& triangleVertices, coTriangulateScratch& scratch)
{
	coASSERT(!coIsClockwiseXY(poly));
	_coPrepareTriangulate(poly, triangleVertices, scratch);

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
						scratch.deviations[vertIdx] = coAbs(coNormalize(prev - cur).z) + coAbs(coNormalize(next - cur).z);

						// # Other attempts:

						// Using the Y delta directly. Does not differientate between very large triangles and small triangles.
						//aDeviations[uPoint] = MATH::Abs(cur.GetY() - prev.GetY()) + MATH::Abs(cur.GetY() - next.GetY());

						// Using the triangle normal. A neighbor of an outliner point can be affected too much but that outlier to the point 
						// where it can have a worse deviation because its triangle is smaller than the outlier's one. 
						//const VECTOR4 vNormal = VECTOR4::Normalize3(VECTOR4::CrossProduct(cur - prev, next - cur));
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
					if (_coIsInsideTriangleXY(prev, cur, next, v))
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
			coPushBack(triangleVertices, prevIdx);
			coPushBack(triangleVertices, curIdx);
			coPushBack(triangleVertices, nextIdx);

			// Remove vertex
			coRemoveOrderedByIndex(scratch.remainingIndices, i);
			break;
		}
	}
	coASSERT(triangleVertices.count % 3 == 0);
	coASSERT(triangleVertices.count / 3 == poly.vertices.count - 2);
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

coBool coContainsFlatVertices(const coPolygon3& poly, coFloat epsilon)
{
	coUint32 prev = poly.vertices.count - 1;
	const coFloatx4 epsilonx4 = epsilon;
	for (coUint32 cur = 0; cur < poly.vertices.count; ++cur)
	{
		coUint32 next = cur + 1;
		if (next == poly.vertices.count)
			next = 0;
		const coVec3& a = poly.vertices[prev];
		const coVec3& b = poly.vertices[cur];
		const coVec3& c = poly.vertices[next];
		const coVec3 ab = coNormalize(b - a);
		const coVec3 bc = coNormalize(c - b);
		const coFloatx4 dot = coDot(ab, bc);
		if (coAbs(dot - coFloatx4(1.0f)) < epsilon)
			return true;
		prev = cur;
	}
	return false;
}

void coRemoveFlatVertices(coPolygon3& poly, coFloat epsilon)
{
	coUint32 prev = poly.vertices.count - 1;
	const coFloatx4 epsilonx4 = epsilon;
	for (coUint32 cur = 0; cur < poly.vertices.count; ++cur)
	{
		coUint32 next = cur + 1;
		if (next == poly.vertices.count)
			next = 0;
		const coVec3& a = poly.vertices[prev];
		const coVec3& b = poly.vertices[cur];
		const coVec3& c = poly.vertices[next];
		const coVec3 ab = coNormalize(b - a);
		const coVec3 bc = coNormalize(c - b);
		const coFloatx4 dot = coDot(ab, bc);
		if (coAbs(dot - coFloatx4(1.0f)) < epsilon)
		{
			coRemoveOrderedByIndex(poly.vertices, cur);
			--cur;
			if (prev == poly.vertices.count)
				prev = poly.vertices.count - 1;
		}
		else
		{
			prev = cur;
		}
	}
}
