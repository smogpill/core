// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coAABBH.h"
class coTriangleSplitter;

class coAABBHBuilder
{
public:
	void Build(coAABBH& aabbh, coTriangleSplitter& splitter);

private:
	using Node = coAABBH::Node;

	void BuildBounds(coAABBH& aabbh, const coTriangleSplitter& splitter);

	/// 4: good for speed, 8: good for memory
	coUint _maxNbObjectsPerLeaf = 4;
};
