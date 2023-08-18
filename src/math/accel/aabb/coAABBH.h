// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <math/shape/coAabb.h>
#include "../../shape/aabb/coAABox4.h"
#include <math/shape/coRay.h>
#include <math/shape/coSphere.h>
#include "../../vector/coUint32x4_f.h"
#include "../../collision/overlap/aabox4/coOverlap_AABox4_AABox4.h"
#include <container/array/coDynamicArray.h>

class coAABBH
{
public:
	static constexpr coUint32 s_maxQueryStackSize = 4 * 1024;
	void FindOverlaps(coDynamicArray<coUint32>& outTriangles, const coAabb& aabb) const;
	void FindOverlaps(coDynamicArray<coUint32>& outTriangles, const coVec3& halfSize, const coRay& ray) const;
	void FindOverlaps(coDynamicArray<coUint32>& outTriangles, const coRay& ray) const;
	template <coUint MAX_NB, class Collector>
	coUint32 FindOverlaps(Collector collector, const coSphere& sphere) const;
	template <class Collector>
	coUint32 FindAABBPathsAtOverlap(Collector collector, const coSphere& sphere) const;

	template <class AcceptNode, class VisitNodes, class VisitObjects>
	void WalkTree(const AcceptNode& acceptNode, const VisitNodes& visitNodes, const VisitObjects& visitObjects) const;

private:
	friend class coAABBTreeBuilder;

	constexpr coUint32 s_objectCountBits = 4;
	constexpr coUint32 s_objectCountShift = 28;
	constexpr coUint32 s_objectCountMask = (1 << s_objectCountBits) - 1;
	constexpr coUint32 s_offsetBits = 28;
	constexpr coUint32 s_offsetMask = (1 << s_offsetBits) - 1;

	struct Node
	{
		coAABox4 _bounds; // TOOD: Should use half instead of float later
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
		const coBool32x4 overlapped = coOverlap_AABox4_AABox4(bounds, aabox4);
	};
	auto visitObjects = [](coUint32 objectsOffset, coUint32 nbObjects)
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
	const coFloatx4 radius = coBroadcastW(sphere.centerAndRadius);
	coAabb aabb(-radius, +radius);
	aabb += coVec3(sphere.centerAndRadius);
	const coAABox4 aabox4(aabb);
	auto acceptNode = [](coInt) { return true; };
	auto visitNodes = [](const coAABox4& bounds, coUint32x4& props, coInt) -> coUint
	{
		const coVec3 center = (nodeAabb.max + nodeAabb.min) * 0.5f;
		const coVec3 d = coAbs(p - center) - (a.max - a.min) * 0.5f;
		return coLength(coMax(d, 0.0f)) + coMin(coBitCast<coFloatx4>(coMax(d)), 0.0f);
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
	if (nodes.count == 0) [[unlikely]]
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
			coUint32x4 props = node._props;
			const coUint nb = visitNodes(node._bounds, props, top);
			coASSERT(nb + 4 <= coARRAY_SIZE(stack));
			coStore(props, &stack[top]);
			top += nb;
		}
		else
		{
			const coUint32 objectsOffset = props & ~(s_objectCountMask << s_objectCountShift);
			visitObjects(objectsOffsets, nbObjects);
		}

		do
			--top;
		while (top >= 0 && !acceptNode(top));
	} while (top >= 0);
}
