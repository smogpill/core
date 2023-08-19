// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coAABBHBuilder.h"
#include "../splitter/coTriangleSplitter.h"
#include "coAABBH.h"

void coAABBHBuilder::Build(coAABBH& aabbh, coTriangleSplitter& splitter)
{
	aabbh.Clear();
	using Range = coTriangleSplitter::Range;

	const Range initialRange = splitter.GetInitialRange();
	if (initialRange.GetSize() <= _maxNbObjectsPerLeaf)
	{
		aabbh._rootProps = BuildObjectList(aabbh, splitter, initialRange);
	}
	else
	{
		coDynamicArray<Node>& nodes = aabbh._nodes;
		coDynamicArray<coUint32>& objects = aabbh._objects;
		struct Work
		{
			Range _range;
			coUint32 _nodeIdx;
		};
		coDynamicArray<Work> stack;

		// Push first node
		{
			coPushBack(nodes, Node());
			Work work;
			work._nodeIdx = 0;
			work._range = initialRange;
			coPushBack(stack, work);
		}

		while (true)
		{
			const Work& work = coPopBack(stack);
			Range left, right;
			splitter.SplitNoFail(work._range, left, right);
			Range ranges[4];
			splitter.SplitNoFail(left, ranges[0], ranges[1]);
			splitter.SplitNoFail(right, ranges[2], ranges[3]);

			Node& node = nodes[work._nodeIdx];
			for (coUint childIdx = 0; childIdx < 4; ++childIdx)
			{
				const Range& childRange = ranges[childIdx];
				const coUint32 nbObjects = childRange.GetSize();
				if (nbObjects > _maxNbObjectsPerLeaf)
				{
					const coUint32 childNodeIdx = nodes.count;
					node._props[childIdx] = childNodeIdx;
					Work childWork;
					childWork._nodeIdx = childNodeIdx;
					childWork._range = childRange;
					coPushBack(stack, childWork);
					coPushBack(nodes, Node());
				}
				else
				{
					node._props[childIdx] = BuildObjectList(aabbh, splitter, childRange);
				}
			}
		}
	}

	coShrinkToFit(aabbh._nodes);
	coShrinkToFit(aabbh._objects);
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
			const coUint32 objectsOffset = props & coAABBH::s_offsetMask;
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

coUint32 coAABBHBuilder::BuildObjectList(coAABBH& aabbh, const coTriangleSplitter& splitter, const Range& range) const
{
	if (range.GetSize() == 0)
		return ~coUint32(0);
	
	coASSERT(range.GetSize() <= _maxNbObjectsPerLeaf);
	coDynamicArray<coUint32>& objects = aabbh._objects;
	const coUint32 objectOffset = objects.count;
	for (coUint32 objectIdx = range._begin; objectIdx < range._end; ++objectIdx)
	{
		const coUint32 objectID = splitter.GetSortedTriangle(objectIdx);
		coPushBack(objects, objectID);
	}
	return (range.GetSize() << coAABBH::s_objectCountShift) | objectOffset;
}
