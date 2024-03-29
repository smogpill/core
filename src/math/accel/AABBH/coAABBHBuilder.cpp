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
	const coUint32 nbObjects = initialRange.GetSize();

#ifdef coDEV
	coClear(_touchedObjects);
	coResize(_touchedObjects, nbObjects, false);
#endif

	coReserve(aabbh._objects, nbObjects);
	coReserve(aabbh._nodes, nbObjects * 3); // arbitrary

	if (nbObjects <= _maxNbObjectsPerLeaf)
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
		coDynamicArray<Range> potentialChildStack[2];

		// Push first node
		{
			coPushBack(nodes, Node());
			Work work;
			work._nodeIdx = 0;
			work._range = initialRange;
			coPushBack(stack, work);
		}

		do
		{
			const Work& work = coPopBack(stack);
			Range ranges[4];
			Build4ChildRanges(work._range, splitter, ranges);
			const coUint32 nodeIdx = work._nodeIdx;
			for (coUint childIdx = 0; childIdx < 4; ++childIdx)
			{
				const Range& childRange = ranges[childIdx];
				const coUint32 nbChildObjects = childRange.GetSize();
				if (nbChildObjects > _maxNbObjectsPerLeaf)
				{
					const coUint32 childNodeIdx = nodes.count;
					nodes[nodeIdx]._props[childIdx] = childNodeIdx;
					coPushBack(nodes, Node());
					Work childWork;
					childWork._nodeIdx = childNodeIdx;
					childWork._range = childRange;
					coPushBack(stack, childWork);
				}
				else
				{
					nodes[nodeIdx]._props[childIdx] = BuildObjectList(aabbh, splitter, childRange);
				}
			}
		} while (stack.count);
	}

	coShrinkToFit(aabbh._nodes);
	coShrinkToFit(aabbh._objects);
	BuildBounds(aabbh, splitter);
#ifdef coDEV
	for (coBool& touchedObject : _touchedObjects)
	{
		coASSERT(touchedObject);
		touchedObject = false;
	}
	auto acceptNode = [](coInt) { return true; };
	auto visitNodes = [](const coAABox4&, coUint32x4&, coInt) -> coUint { return 4; };
	auto visitObjects = [this, &aabbh](coUint32 objectsOffset, coUint32 nbObjects)
	{
		for (coUint i = 0; i < nbObjects; ++i)
		{
			const coUint32 objectIdx = aabbh._objects[objectsOffset + i];
			coASSERT(_touchedObjects[objectIdx] == false);
			_touchedObjects[objectIdx] = true;
		}
		return true;
	};
	aabbh.WalkTree(acceptNode, visitNodes, visitObjects);
	for (coBool& touchedObject : _touchedObjects)
	{
		coASSERT(touchedObject);
	}
#endif
}

void coAABBHBuilder::Build4ChildRanges(const Range& initialRange, coTriangleSplitter& splitter, Range* ranges)
{
	coClear(_scratchRanges[0]);
	coUint currentStack = 0;
	coPushBack(_scratchRanges[0], initialRange);
	coUint curStackIdx = 0;
	coUint nextStackIdx = 1;
	coUint nbRangesFound = 1;
	coBool split = false;
	do
	{
		auto& nextRanges = _scratchRanges[nextStackIdx];
		coClear(nextRanges);
		split = false;
		for (const Range& curRange : _scratchRanges[curStackIdx])
		{
			if (curRange.GetSize() <= _maxNbObjectsPerLeaf || nbRangesFound == 4)
			{
				coPushBack(nextRanges, curRange);
				continue;
			}
			Range left, right;
			splitter.SplitNoFail(curRange, left, right);
			coASSERT(left.GetSize() > 0);
			coASSERT(right.GetSize() > 0);
			coPushBack(nextRanges, left);
			coPushBack(nextRanges, right);
			split = true;
			++nbRangesFound;
		}

		coSwap(curStackIdx, nextStackIdx);
	} while (nbRangesFound < 4 && split);

	auto& currentRanges = _scratchRanges[curStackIdx];

	// Split current ranges further down to balance through the remaining slots
	// (Since we already have them allocated, better use them)
	while (nbRangesFound != 4)
	{
		coUint maxIdx = 0;
		for (coUint j = 1; j < nbRangesFound; ++j)
			if (currentRanges[j].GetSize() > currentRanges[maxIdx].GetSize())
				maxIdx = j;
		const Range& maxRange = currentRanges[maxIdx];
		if (maxRange.GetSize() < 2)
			break;

		Range left, right;
		splitter.SplitNoFail(maxRange, left, right);
		coASSERT(left.GetSize() > 0);
		coASSERT(right.GetSize() > 0);
		coRemoveUnorderedByIndex(currentRanges, maxIdx);
		coPushBack(currentRanges, left);
		coPushBack(currentRanges, right);
		++nbRangesFound;
	}

	for (coUint i = 0; i < nbRangesFound; ++i)
		ranges[i] = currentRanges[i];

	for (coUint i = nbRangesFound; i < 4; ++i)
		ranges[i] = Range(0, 0);
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
		else if (props != ~coUint32(0))
		{
			const coUint32 objectsOffset = props & coAABBH::s_offsetMask;
			for (coUint32 o = 0; o < nbObjects; ++o)
			{
				const coUint32 objectID = objects[objectsOffset + o];
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
			coSetElement(node._bounds, childIdx, buildAABB(props));
		}
	}

	// Build the root aabb
	aabbh._rootAabb = buildAABB(aabbh._rootProps);
}

coUint32 coAABBHBuilder::BuildObjectList(coAABBH& aabbh, const coTriangleSplitter& splitter, const Range& range)
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
#ifdef coDEV
		coASSERT(!_touchedObjects[objectID]);
		_touchedObjects[objectID] = true;
#endif
	}
	return (range.GetSize() << coAABBH::s_objectCountShift) | objectOffset;
}
