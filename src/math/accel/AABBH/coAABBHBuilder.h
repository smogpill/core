// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coAABBH.h"
#include "../splitter/coTriangleSplitter.h"

class coAABBHBuilder
{
public:
	void Build(coAABBH& aabbh, coTriangleSplitter& splitter);

private:
	using Node = coAABBH::Node;
	using Range = coTriangleSplitter::Range;

	void Build4ChildRanges(const Range& initialRange, coTriangleSplitter& splitter, Range* ranges);
	void BuildBounds(coAABBH& aabbh, const coTriangleSplitter& splitter);
	coUint32 BuildObjectList(coAABBH& aabbh, const coTriangleSplitter& splitter, const Range& range) const;

	/// 4: good for speed, 8: good for memory
	coUint _maxNbObjectsPerLeaf = 4;
	coDynamicArray<Range> _scratchRanges[2];
};
