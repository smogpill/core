// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coAABBHBuilder.h"
#include "../splitter/coTriangleSplitter.h"
#include "coAABBH.h"

void coAABBHBuilder::Build(coAABBH& aabbh, coTriangleSplitter& splitter)
{
	aabbh._rootProps = 0;
	using Range = coTriangleSplitter::Range;
	coDynamicArray<Range> stack;
	coDynamicArray<Node> nodes;
	coDynamicArray<coUint32> objects;
	coDynamicArray<coUint32> parents;
	while (true)
	{
		const Range& range = coPopBack(stack);
		Range left, right;
		splitter.SplitNoFail(range, left, right);
		Range ranges[4];
		splitter.SplitNoFail(left, ranges[0], ranges[1]);
		splitter.SplitNoFail(right, ranges[1], ranges[2]);

		Node node;
		for (coUint childIdx = 0; childIdx < 4; ++childIdx)
		{
			const Range& childRange = ranges[childIdx];
			const coUint32 nbObjects = childRange.GetSize();
			if (nbObjects > _maxNbObjectsPerLeaf)
			{
				const coUint32 childNodeIdx = nodes.count;
				coPushBack(nodes, Node());
				node._props[childIdx] = childNodeIdx;
				coPushBack(stack, childRange);
			}
			else
			{
				const coUint32 objectOffset = objects.count;
				node._props[childIdx] = (nbObjects << coAABBH::s_objectCountShift) | objectOffset; 
				for (coUint32 objectIdx = childRange._begin; objectIdx < childRange._end; ++objectIdx)
				{
					const coUint32 objectID = splitter.GetSortedTriangle(objectIdx);
					coPushBack(objects, objectID);
				}
			}
		}
	}

	// No std::move because we want arrays to shrink their allocations.
	aabbh._nodes = nodes;
	aabbh._objects = objects;

	BuildBounds(aabbh, splitter);
}

void coAABBHBuilder::BuildBounds(coAABBH& aabbh, const coTriangleSplitter& splitter)
{
	coArray<Node>& nodes = aabbh._nodes;
	const coArray<coUint32>& objects = aabbh._objects;

	auto buildAABB = [&](const coUint32 props)
	{
		const coUint32 nbObjects = props >> coAABBH::s_objectCountShift;
		coAabb bounds;
		if (nbObjects == 0)
		{
			const Node& childNode = nodes[props];
			bounds = coMerge(childNode._bounds);
		}
		else
		{
			const coUint32 objectsOffset = props & ~(coAABBH::s_objectCountMask << coAABBH::s_objectCountShift);
			for (coUint32 o = 0; o < nbObjects; ++o)
			{
				const coUint32 objectID = splitter.GetSortedTriangle(objects[objectsOffset + o]);
				const coAabb objectAABB = splitter.GetObjectAABB(objectID);
				bounds = coMerge(bounds, objectAABB);
			}
		}
		return bounds;
	};

	// We build the bounds in reverse order, from leaves to parents.
	for (coInt32 nodeIdx = coInt32(nodes.count) - 1; nodeIdx >= 0; --nodeIdx)
	{
		Node& node = nodes[nodeIdx];
		for (coUint childIdx = 0; childIdx < 4; ++childIdx)
		{
			const coUint32 props = node._props[childIdx];
			node._bounds[childIdx] = buildAABB(props);
		}
	}

	// Build the root aabb
	aabbh._rootAabb = buildAABB(aabbh._rootProps);
}
