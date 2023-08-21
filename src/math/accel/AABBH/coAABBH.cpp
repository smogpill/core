// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coAABBH.h"
#include <container/array/coDynamicArray_f.h>
#include "../../shape/ray/coRayx4.h"
#include "../../collision/overlap/aabox4/coOverlap_AABox4_Rayx4.h"

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
	MovingAABB movingAabb;
	movingAabb.ray.origin = ray_.origin;
	// We add some epsilon because Recipral seems to create NaN instead of inf with the 0 components of the direction.
	movingAabb.ray.invDir = coInvert(ray_.GetDir() + coVec3(1e-20f));
	coASSERT(coIsValid(movingAabb.ray.invDir));
	movingAabb.ray.len = coSplatW(ray_.dirAndLength);
	movingAabb.halfSize = halfSize_;
}

void coAABBH::FindOverlaps(coDynamicArray<coUint32>& objects, const coRay& ray) const
{
	coRay2x4 ray2;
	ray2._origin = ray.origin;
	// We add some epsilon because Recipral seems to create NaN instead of inf with the 0 components of the direction.
	ray2._invDir = coInvert(ray.GetDir() + coVec3(1e-20f));
	ray2._length = coSplatW(ray.dirAndLength);

	auto acceptNode = [](coInt) { return true; };
	auto visitNodes = [ray2](const coAABox4& bounds, coUint32x4& props, coInt) -> coUint
	{
		const coBool32x4 overlapped = coOverlapSolidSolid(bounds, ray2);
		props = coSortTrueFirst(props, overlapped);
		return coCountTrues(overlapped);
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
