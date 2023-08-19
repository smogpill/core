// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coAABBH.h"
#include <container/array/coDynamicArray_f.h>

void coAABBH::Clear()
{
	_rootAabb = coAabb();
	_rootProps = 0;
	coClear(_nodes);
	coClear(_objects);
}

void coAABBH::FindOverlaps(coDynamicArray<coUint32>& objects, const coAabb& aabb) const
{
	auto collector = [&objects](coUint32 objectIdx)
	{
		coASSERT(!coContains(objects, objectIdx));
		coPushBack(objects, objectIdx);
	};
	FindOverlaps(collector, aabb);
}

void coAABBH::FindOverlaps(coDynamicArray<coUint32>& objects, const coVec3& halfSize, const coRay& ray) const
{
}

void coAABBH::FindOverlaps(coDynamicArray<coUint32>& objects, const coRay& ray) const
{
	auto acceptNode = [](coInt) { return true; };
	auto visitNodes = [](const coAABox4& bounds, coUint32x4& props, coInt) -> coUint
	{
		/*
		const coBool32x4 overlapped = coOverlapSolidSolid(bounds, ray);
		props = coSortTrueFirst(props, overlapped);
		return coCountTrues(overlapped);
		*/
		return false;
	};
	auto visitObjects = [this, &objects](coUint32 objectsOffset, coUint32 nbObjects)
	{
		for (coUint i = 0; i < nbObjects; ++i)
		{
			const coUint32 objectIdx = _objects[objectsOffset + i];
			coASSERT(!coContains(objects, objectIdx));
			coPushBack(objects, objectIdx);
		}
		return true;
	};
	WalkTree(acceptNode, visitNodes, visitObjects);
}
