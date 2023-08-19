// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coTriangleSplitterBinning.h"
#include <container/array/coDynamicArray_f.h>
#include <math/vector/coVec3_f.h>
#include <math/shape/coAabb_f.h>

// Based on Jorrit Rouwe's JoltPhysics' TriangleSplitterBinning

coTriangleSplitterBinning::coTriangleSplitterBinning(const coArray<coVec3>& vertices, const coArray<coUint32>& indices, coUint32 minNbBins, coUint32 maxNbBins, coUint32 nbTrianglesPerBin)
	: Base(vertices, indices)
	, _minNbBins(minNbBins)
	, _maxNbBins(maxNbBins)
	, _nbTrianglesPerBin(nbTrianglesPerBin)
{
	coResize(_bins, _maxNbBins);
}

coBool coTriangleSplitterBinning::Split(const Range& inTriangles, Range& outLeft, Range& outRight)
{
	// Calculate bounds for this range
	coAabb centroid_bounds;
	for (coUint32 t = inTriangles._begin; t < inTriangles._end; ++t)
		centroid_bounds = coMerge(centroid_bounds, _centroids[_sortedTriangles[t]]);

	coFloat best_cp = FLT_MAX;
	coUint32 best_dim = 0xffffffff;
	coFloat best_split = 0;

	// Bin in all dimensions
	coUint32 num_bins = coClamp(inTriangles.GetSize() / _nbTrianglesPerBin, _minNbBins, _maxNbBins);
	for (coUint dim = 0; dim < 3; ++dim)
	{
		coFloat bounds_min = centroid_bounds.min[dim];
		coFloat bounds_size = centroid_bounds.max[dim] - bounds_min;

		// Skip axis if too small
		if (bounds_size < 1.0e-5f)
			continue;

		// Initialize bins
		for (coUint32 b = 0; b < num_bins; ++b)
		{
			Bin& bin = _bins[b];
			coClear(bin._bounds);
			bin._minCentroid = bounds_min + bounds_size * (b + 1) / num_bins;
			bin._nbTriangles = 0;
		}

		// Bin all triangles
		for (coUint32 t = inTriangles._begin; t < inTriangles._end; ++t)
		{
			coFloat centroid_pos = _centroids[_sortedTriangles[t]][dim];

			// Select bin 
			coUint32 bin_no = coMin(coUint32((centroid_pos - bounds_min) / bounds_size * num_bins), num_bins - 1);
			Bin& bin = _bins[bin_no];

			// Accumulate triangle in bin
			const coUint32 sortedTriangleIdx = _sortedTriangles[t];
			const coUint32 sortedTriangleFirstIdx = sortedTriangleIdx * 3;
			bin._bounds = coMerge(bin._bounds, _vertices[_indices[sortedTriangleFirstIdx + 0]]);
			bin._bounds = coMerge(bin._bounds, _vertices[_indices[sortedTriangleFirstIdx + 1]]);
			bin._bounds = coMerge(bin._bounds, _vertices[_indices[sortedTriangleFirstIdx + 2]]);
			bin._minCentroid = coMin(bin._minCentroid, centroid_pos);
			bin._nbTriangles++;
		}

		// Calculate totals left to right
		coAabb prev_bounds;
		int prev_triangles = 0;
		for (coUint32 b = 0; b < num_bins; ++b)
		{
			Bin& bin = _bins[b];
			bin._boundsAccumulatedLeft = prev_bounds; // Don't include this node as we'll take a split on the left side of the bin
			bin._nbTrianglesAccumulatedLeft = prev_triangles;
			prev_bounds = coMerge(prev_bounds, bin._bounds);
			prev_triangles += bin._nbTriangles;
		}

		// Calculate totals right to left
		coClear(prev_bounds);
		prev_triangles = 0;
		for (int b = num_bins - 1; b >= 0; --b)
		{
			Bin& bin = _bins[b];
			prev_bounds = coMerge(prev_bounds, bin._bounds);
			prev_triangles += bin._nbTriangles;
			bin._boundsAccumulatedRight = prev_bounds;
			bin._nbTrianglesAccumulatedRight = prev_triangles;
		}

		// Get best splitting plane
		for (coUint b = 1; b < num_bins; ++b) // Start at 1 since selecting bin 0 would result in everything ending up on the right side
		{
			// Calculate surface area heuristic and see if it is better than the current best
			const Bin& bin = _bins[b];
			float cp = coGetSurfaceArea(bin._boundsAccumulatedLeft) * bin._nbTrianglesAccumulatedLeft + coGetSurfaceArea(bin._boundsAccumulatedRight) * bin._nbTrianglesAccumulatedRight;
			if (cp < best_cp)
			{
				best_cp = cp;
				best_dim = dim;
				best_split = bin._minCentroid;
			}
		}
	}

	// No split found?
	if (best_dim == 0xffffffff)
		return false;

	return SplitInternal(inTriangles, best_dim, best_split, outLeft, outRight);
}
