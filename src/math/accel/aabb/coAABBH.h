// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <math/shape/coAabb.h>
#include "../../shape/aabb/coAABox4.h"
#include <math/shape/coRay.h>
#include <math/shape/coSphere.h>
#include "../../vector/coUint32x4_f.h"
#include "../../collision/overlap/aabox4/coOverlap_AABox4_AABox4.h"
#include "../../collision/overlap/aabox4/coOverlap_AABox4_Sphere.h"
#include <container/array/coDynamicArray.h>

class coAABBH
{
public:
	static constexpr coUint32 s_maxQueryStackSize = 4 * 1024;

	void Clear();
	void FindOverlaps(coDynamicArray<coUint32>& objects, const coAabb& aabb) const;
	void FindOverlaps(coDynamicArray<coUint32>& objects, const coVec3& halfSize, const coRay& ray) const;
	void FindOverlaps(coDynamicArray<coUint32>& objects, const coRay& ray) const;
	template <class Collector>
	void FindOverlaps(Collector collector, const coAabb& aabox) const;
	template <class Collector>
	void FindOverlaps(Collector collector, const coSphere& sphere) const;
	template <class AcceptNode, class VisitNodes, class VisitObjects>
	void WalkTree(const AcceptNode& acceptNode, const VisitNodes& visitNodes, const VisitObjects& visitObjects) const;

private:
	friend class coAABBHBuilder;

	static constexpr coUint32 s_objectCountBits = 4;
	static constexpr coUint32 s_objectCountShift = 28;
	static constexpr coUint32 s_objectCountMask = (1 << s_objectCountShift) - 1;
	static constexpr coUint32 s_offsetBits = 28;
	static constexpr coUint32 s_offsetMask = (1 << s_offsetBits) - 1;

	struct Node
	{
		coAABox4 _bounds; // TODO: Should use half instead of float later
		coUint32x4 _props;
	};

	coAabb _rootAabb;
	coUint32 _rootProps = 0;
	coDynamicArray<Node> _nodes;
	coDynamicArray<coUint32> _objects;
};

template <class COLLECTOR>
void coAABBH::FindOverlaps(COLLECTOR collector, const coAabb& aabox) const
{
	const coAABox4 aabox4(aabox);
	auto acceptNode = [](coInt) { return true; };
	auto visitNodes = [aabox4](const coAABox4& bounds, coUint32x4& props, coInt stackTop) -> coUint
	{
		const coBool32x4 overlapped = coOverlapSolidSolid(bounds, aabox4);
		props = coSortTrueFirst(props, overlapped);
		return coCountTrues(overlapped);
	};
	auto visitObjects = [this, &collector](coUint32 objectsOffset, coUint32 nbObjects)
	{
		for (coUint i = 0; i < nbObjects; ++i)
			collector(_objects[objectsOffset + i]);
		return true;
	};
	WalkTree(acceptNode, visitNodes, visitObjects);
}

template <class COLLECTOR>
void coAABBH::FindOverlaps(COLLECTOR collector, const coSphere& sphere) const
{
	auto acceptNode = [](coInt) { return true; };
	auto visitNodes = [](const coAABox4& bounds, coUint32x4& props, coInt) -> coUint
	{
		const coBool32x4 overlapped = coOverlapSolidSolid(bounds, sphere);
		props = coSortTrueFirst(props, overlapped);
		return coCountTrues(overlapped);
	};
	auto visitObjects = [](coUint32 objectsOffset, coUint32 nbObjects)
	{
		for (coUint i = 0; i < nbObjects; ++i)
			collector(_objects[objectsOffset + i]);
		return true;
	};
	WalkTree(acceptNode, visitNodes, visitObjects);
}

template <class AcceptNode, class NodesVisitor, class ObjectsVisitor>
void coAABBH::WalkTree(const AcceptNode& acceptNode, const NodesVisitor& visitNodes, const ObjectsVisitor& visitObjects) const
{
	if (_nodes.count == 0) [[unlikely]]
		return;

	coUint32 stack[s_maxQueryStackSize];
	stack[0] = _rootProps;
	coInt top = 0;

	do
	{
		const coUint32 props = stack[top];
		const coUint32 nbObjects = props >> s_objectCountShift;
		if (nbObjects == 0) [[likely]]
		{
			const Node& node = _nodes[props];
			coUint32x4 childProps = node._props;
			const coUint nb = visitNodes(node._bounds, childProps, top);
			coASSERT(nb + 4 <= coARRAY_SIZE(stack));
			coStore(childProps, &stack[top]);
			top += nb;
		}
		else if (props != ~~coUint32(0))
		{
			const coUint32 objectsOffset = props & s_offsetMask;
			visitObjects(objectsOffset, nbObjects);
		}

		do
			--top;
		while (top >= 0 && !acceptNode(top));
	} while (top >= 0);
}
