// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <math/shape/coAabb.h>
#include "coTriangleSplitter.h"

/// Based on Jorrit Rouwe's JoltPhysics' TriangleSplitterBinning
/// Binning splitter approach taken from: Realtime Ray Tracing on GPU with BVH-based Packet Traversal by Johannes Gunther et al.
class coTriangleSplitterBinning final : public coTriangleSplitter
{
	using Base = coTriangleSplitter;
public:
	coTriangleSplitterBinning(const coArray<coVec3>& vertices, const coArray<coUint32>& indices, coUint32 inMinNumBins = 8, coUint32 inMaxNumBins = 128, coUint32 inNumTrianglesPerBin = 6);

	coBool Split(const Range& inTriangles, Range& outLeft, Range& outRight) override;
private:
	// Configuration
	const coUint32 _minNbBins;
	const coUint32 _maxNbBins;
	const coUint32 _nbTrianglesPerBin;

	struct Bin
	{
		// Properties of this bin
		coAabb _bounds;
		coFloat _minCentroid;
		coUint32 _nbTriangles;

		// Accumulated data from left most / right most bin to current (including this bin)
		coAabb _boundsAccumulatedLeft;
		coAabb _boundsAccumulatedRight;
		coUint32 _nbTrianglesAccumulatedLeft;
		coUint32 _nbTrianglesAccumulatedRight;
	};

	// Scratch area to store the bins
	coDynamicArray<Bin> _bins;
};
