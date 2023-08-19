// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coPolygon3_f.h"
#include "math/vector/coVec3_f.h"
#include "math/shape/triangle/coTriangle_f.h"
#include <container/array/coDynamicArray_f.h>

void _coPrepareTriangulate(const coPolygon3& poly, coDynamicArray<coUint32>& triangles, coTriangulateScratch& scratch)
{
	coClear(scratch.sorted);
	coClear(scratch.concaves);
	coClear(scratch.deviations);
	coClear(triangles);
	const coUint32 nbPoints = poly.vertices.count;

	// Copy the provided points to a dynamic list to work on.
	{
		coClearAndResize(scratch.remainingVertices, nbPoints);
		for (coUint32 i = 0; i < nbPoints; ++i)
			scratch.remainingVertices[i] = i;
	}
}

coFORCE_INLINE coBool _coIsCornerConvexXY(const coVec3& a, const coVec3& b, const coVec3& c)
{
	return !coAreAllFalse(coSplatZ(coGetRawNormal(a, b, c)) >= coFloatx3(0.0f));
}
coFORCE_INLINE coBool _coIsInsideTriangleXY(const coVec3& a, const coVec3& b, const coVec3& c, const coVec3& p)
{
	const coBool b1 = _coIsCornerConvexXY(p, a, b);
	const coBool b2 = _coIsCornerConvexXY(p, b, c);
	const coBool b3 = _coIsCornerConvexXY(p, c, a);

	return (b1 == b2) & (b2 == b3);
}

coFORCE_INLINE coBool _coIsConcave(const coVec3& a, const coVec3& b, const coVec3& c, const coVec3& normal)
{
	return coDot(coGetRawNormal(a, b, c), normal).x < 0.f;
}

void coTriangulateAssumingFlat(const coPolygon3& poly, coDynamicArray<coUint32>& triangleVertices, coTriangulateScratch& scratch, const coVec3& planeNormal)
{
	// TODO: 
	// - Project the vertices to the plane to work directly in 2d
	// - Use some acceleration structure to find overlaps (Blender uses a KD-tree)
	// - Precompute convexity (don't forget to update it when an ear is removed)
	// - To improve quality: Check Ear-clipping Based Algorithms of Generating High - quality Polygon Triangulation Gang Mei1, John C.Tipper1 and Nengxiong Xu

	// References: 
	// - FIST: Fast Industrial-Strength Triangulation of Polygons https://www.cosy.sbg.ac.at/~held/projects/triang/triang.html
	// - Fast Polygon Triangulation Based on Seidel's Algorithm http://gamma.cs.unc.edu/SEIDEL/#:~:text=It%20is%20an%20incremental%20randomized,polygon%20into%20n%2D2%20triangles.
	// - Blender : BLI_polyfill_calc_arena
	// - blender/blenlib/intern/polyfill_2d.c

	const coFloat epsilon = 1e-4f;

	const auto& vertices = poly.vertices;
	auto& remainingVertices = scratch.remainingVertices;

	if (vertices.count == 3)
	{
		coClearAndResize(triangleVertices, 3);
		triangleVertices[0] = 0;
		triangleVertices[1] = 1;
		triangleVertices[2] = 2;
		return;
	}
	else if (vertices.count < 3)
	{
		coClear(triangleVertices);
		return;
	}

	// Call is consistent/robust
	auto getSide = [&](const coUint32 a, const coUint32 b, const coUint32 c)
	{
		// We reorder from increasing indices, for consistency.
		coUint32 orderedA = a;
		coUint32 orderedB = b;
		coUint32 orderedC = c;

		coBool side = true;
		if (orderedA > orderedC)
		{
			coSwap(orderedA, orderedC);
			side = !side;
		}
		if (orderedA > orderedB)
		{
			coSwap(orderedA, orderedB);
			side = !side;
		}
		if (orderedB > orderedC)
		{
			coSwap(orderedB, orderedC);
			side = !side;
		}

		const coVec3& vertA = vertices[orderedA];
		const coVec3& vertB = vertices[orderedB];
		const coVec3& vertC = vertices[orderedC];
		const coFloat val = coDot(coGetRawNormal(vertA, vertB, vertC), planeNormal).x;
		if (coAbs(val) > epsilon)
			return (val > 0.0f) == side ? 1 : -1;
		else
			return 0;
	};

	auto isConcave = [&](const coUint32 a, const coUint32 b, const coUint32 c)
	{
		return getSide(a, b, c) <= 0;
	};

	auto isInsideTriangle = [&](const coUint32 a, const coUint32 b, const coUint32 c, const coUint32 p)
	{
		// Very important: we need to use getSide to keep overall consistency.
		return getSide(a, b, p) >= 0 && getSide(b, c, p) >= 0 && getSide(c, a, p) >= 0;
	};
	
	//coASSERT(coIsCounterClockwiseXY(poly));
	_coPrepareTriangulate(poly, triangleVertices, scratch);

	const coUint32 expectedNbTriangles = vertices.count - 2;
	coReserve(triangleVertices, expectedNbTriangles * 3);

	// Remove "ear" triangles one by one.
	while (remainingVertices.count > 3)
	{
		coUint32 earIdx = coUint32(-1);

		// Find ear vertex
		for (coUint32 i = 0; i < remainingVertices.count; ++i)
		{
			const coUint32 remainingPrev = i == 0 ? (remainingVertices.count - 1) : (i - 1);
			const coUint32 remainingCur = i;
			const coUint32 remainingNext = i == (remainingVertices.count - 1) ? 0 : (i + 1);
			const coUint32 prevIdx = remainingVertices[remainingPrev];
			const coUint32 curIdx = remainingVertices[remainingCur];
			const coUint32 nextIdx = remainingVertices[remainingNext];
			const coVec3& prev = vertices[prevIdx];
			const coVec3& cur = vertices[curIdx];
			const coVec3& next = vertices[nextIdx];

			// Skip concave vertices
			if (isConcave(prevIdx, curIdx, nextIdx))
				continue;

			// If any other point is inside the triangle, continue
			{
				coBool ignore = false;
				for (coUint32 j = 0; j < remainingVertices.count; ++j)
				{
					// Skip vertices of the current triangle
					if (j == remainingPrev || j == remainingCur || j == remainingNext)
						continue;

					const coUint32 jPrevIdx = remainingVertices[j == 0 ? (remainingVertices.count - 1) : (j - 1)];
					const coUint32 jIdx = remainingVertices[j];
					const coUint32 jNextIdx = remainingVertices[j == (remainingVertices.count - 1) ? 0 : (j + 1)];

					// Skip convex vertices
					if (!isConcave(jPrevIdx, jIdx, jNextIdx))
						continue;

					const coVec3& v = vertices[jIdx];
					if (isInsideTriangle(prevIdx, curIdx, nextIdx, jIdx))
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

			earIdx = i;
			break;
		}

		// No ear found? That means that the polygon is degenerate (e.g. nearly collinear)
		// Note that the provided polygon was not necessarily degenerate at first.
		if (earIdx == coUint32(-1))
		{
			for (coUint32 i = 0; i < remainingVertices.count; ++i)
			{
				const coUint32 remainingPrev = i == 0 ? (remainingVertices.count - 1) : (i - 1);
				const coUint32 remainingCur = i;
				const coUint32 remainingNext = i == (remainingVertices.count - 1) ? 0 : (i + 1);
				const coUint32 prevIdx = remainingVertices[remainingPrev];
				const coUint32 curIdx = remainingVertices[remainingCur];
				const coUint32 nextIdx = remainingVertices[remainingNext];

				// Concave vertex?
				if (isConcave(prevIdx, curIdx, nextIdx))
				{
					continue;
				}

				earIdx = i;
				break;
			}

			// No convex found, just return the first one
			// Note: we don't care anymore if it's concave
			if (earIdx == coUint32(-1))
			{
				earIdx = 0;
			}
		}

		const coUint32 remainingPrev = earIdx == 0 ? (remainingVertices.count - 1) : (earIdx - 1);
		const coUint32 remainingCur = earIdx;
		const coUint32 remainingNext = earIdx == (remainingVertices.count - 1) ? 0 : (earIdx + 1);

		coPushBack(triangleVertices, remainingVertices[remainingPrev]);
		coPushBack(triangleVertices, remainingVertices[remainingCur]);
		coPushBack(triangleVertices, remainingVertices[remainingNext]);

		coRemoveOrderedByIndex(remainingVertices, earIdx);
	}

	coASSERT(remainingVertices.count == 3);
	coPushBack(triangleVertices, remainingVertices[0]);
	coPushBack(triangleVertices, remainingVertices[1]);
	coPushBack(triangleVertices, remainingVertices[2]);

	coASSERT(triangleVertices.count == expectedNbTriangles * 3);
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
			const coUint32 nbRemainingVertices = scratch.remainingVertices.count;
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
					const coVec3& prev = poly.vertices[scratch.remainingVertices[prevIdx]];
					const coVec3& cur = poly.vertices[scratch.remainingVertices[vertIdx]];
					const coVec3& next = poly.vertices[scratch.remainingVertices[nextIdx]];

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
			const coUint32 remainingPrevIdx = i == 0 ? (scratch.remainingVertices.count - 1) : (i - 1);
			const coUint32 remainingCurIdx = i;
			const coUint32 remainingNextIdx = i == (scratch.remainingVertices.count - 1) ? 0 : (i + 1);
			const coUint32 prevIdx = scratch.remainingVertices[remainingPrevIdx];
			const coUint32 curIdx = scratch.remainingVertices[remainingCurIdx];
			const coUint32 nextIdx = scratch.remainingVertices[remainingNextIdx];
			const coVec3& prev = poly.vertices[prevIdx];
			const coVec3& cur = poly.vertices[curIdx];
			const coVec3& next = poly.vertices[nextIdx];

			// If interior vertex, continue
			{
				const coVec3 v1 = prev - cur;
				const coVec3 v2 = next - cur;
				const coVec3 cross = coGetRawNormal(prev, cur, next);
				if (cross[1] <= 0.f)
				{
					continue;
				}
			}

			// If any other point is inside the triangle, continue
			{
				coBool ignore = false;
				for (coUint32 j = 0; j < scratch.remainingVertices.count; ++j)
				{
					// Skip the current triangle
					if (j == remainingPrevIdx || j == remainingCurIdx || j == remainingNextIdx)
						continue;

					const coUint32 n = scratch.remainingVertices[j];
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
			coRemoveOrderedByIndex(scratch.remainingVertices, i);
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
