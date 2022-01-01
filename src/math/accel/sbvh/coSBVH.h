// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray.h>
#include <math/vector/coVec3_f.h>
#include <math/vector/coFloatx4_f.h>
#include <math/shape/coAabb.h>
#include <math/shape/coOverlap_f.h>
class coRay;
class coSphere;
class coTriangle;

class coSBVH
{
public:
	void Build(const coArray<coVec3>& vertices, const coArray<coUint32>& indices);
	void FindOverlaps(coDynamicArray<coUint32>& outTriangles, const coAabb& aabb) const;
	void FindOverlaps(coDynamicArray<coUint32>& outTriangles, const coVec3& halfSize, const coRay& ray) const;
	void FindOverlaps(coDynamicArray<coUint32>& outTriangles, const coRay& ray) const;
	template <coUint MAX_NB, class COLLECTOR>
	void FindOverlaps(COLLECTOR collector, const coSphere& sphere) const;

private:
	struct Node
	{
		coAabb aabb;
		coBool leaf = false;
		coUint32 datas[2] = {};
	};
	struct Ref
	{
		coUint32 triangle = coUint32(-1);
		mutable coUint32 flags = 0;
		coAabb aabb;
	};
	struct Ray2
	{
		coVec3 origin;
		coVec3 invDir;
		coFloatx4 len;
	};
	struct MovingAABB
	{
		Ray2 ray;
		coVec3 halfSize;
	};
	template <class COLLECTOR, class T>
	void FindOverlapsImpl(COLLECTOR collector, const T& query) const;
	coUint32 BuildRec(const coArray<Ref>& refs, const coArray<coAabb>& primitives, coUint32 depth);
	void Split(coAabb* left, coAabb* right, const coUint32 primIndex, coUint8 axis, coFloat pos);
	static coBool Overlap(const coAabb& a, const coAabb& b);
	static coBool Overlap(const coAabb& a, const Ray2& b);
	static coBool Overlap(const coAabb& a, const MovingAABB& b);

	static const coUint32 maxDepth = 512;
	const coArray<coVec3>* vertices = nullptr;
	const coArray<coUint32>* indices = nullptr;
	coDynamicArray<coAabb> aabbs;
	coDynamicArray<Node> nodes;
	coDynamicArray<Ref> refs;
	coUint32 depth = 0;
};


template <class COLLECTOR, class T>
coFORCE_INLINE void coSBVH::FindOverlapsImpl(COLLECTOR collector, const T& query) const
{
	if (nodes.count == 0)
		return;

	// Init stack
	// Note: Allocating 2 x maxDepth for security, but the real value should be 
	// close to maxDepth because we are depth-first.
	static_assert(maxDepth <= 1024, ""); // Should be reasonable, or else a different alloc strategy should be preferred.
	coUint32 stack[2 * maxDepth];
	stack[0] = 0;
	coUint32 stackIdx = 0;

	// Depth-first visit
	do
	{
		const coUint32 nodeIdx = stack[stackIdx--];
		const Node& node = nodes[nodeIdx];
		if (!Overlap(node.aabb, query))
		{
			continue;
		}
		if (node.leaf)
		{
			const coUint32 nbRefs = node.datas[0];
			const coUint32 firstRef = node.datas[1];
			for (coUint32 u = 0; u < nbRefs; ++u)
			{
				const Ref& ref = refs[firstRef + u];
				collector(ref.triangle);
			}
		}
		else
		{
			coASSERT(stackIdx + 2u < coARRAY_SIZE(stack));
			stack[++stackIdx] = node.datas[0];
			stack[++stackIdx] = node.datas[1];
		}
	} while (stackIdx != coUint32(-1));
}

coFORCE_INLINE coBool coSBVH::Overlap(const coAabb& a, const coAabb& b)
{
	return coOverlapSolidSolid(a, b);
}

coFORCE_INLINE coBool coSBVH::Overlap(const coAabb& a, const MovingAABB& b)
{
	coAabb aabb(a);
	aabb.min = aabb.min - b.halfSize;
	aabb.max = aabb.max + b.halfSize;
	return Overlap(aabb, b.ray);
}

coFORCE_INLINE coBool coSBVH::Overlap(const coAabb& a, const Ray2& ray)
{
	// Refs:
	// - Another View on the Classic Ray-AABB Intersection Algorithm for BVH Traversal, Roman Wiche
	// https://medium.com/@bromanz/another-view-on-the-classic-ray-aabb-intersection-algorithm-for-bvh-traversal-41125138b525
	// - Fast, Branchless Ray/Bounding Box Intersections, Tavian Barnes
	// https://tavianator.com/2011/ray_box.html
	// https://tavianator.com/2015/ray_box_nan.html

	const coVec3 t1 = (a.min - ray.origin) * ray.invDir;
	const coVec3 t2 = (a.max - ray.origin) * ray.invDir;
	const coVec3 tmin1 = coMin(t1, t2);
	const coVec3 tmax1 = coMax(t1, t2);
	const coFloat tmin = coMax(0.0f, coMax(tmin1).x);
	const coFloat tmax = coMin(ray.len.x, coMin(tmax1).x);
	return tmin <= tmax;

	// Inigo's solution might not work when the ray starts inside the cube (not sure though, should be analyzed).
	// https://www.iquilezles.org/www/articles/intersectors/intersectors.htm
	// 		const VECTOR4 n = invDir*ray.GetOrigin();
	// 		const VECTOR4 k = VECTOR4::Abs(invDir)*a.GetSizeVector() * 0.5f;
	// 		const VECTOR4 t1 = -n - k;
	// 		const VECTOR4 t2 = -n + k;
	// 		const Float32 tN = t1.GetMaxValue3();
	// 		const Float32 tF = t2.GetMinValue3();
	// 		return tN <= tF && tF >= 0.0f;
}

template <coUint MAX_NB, class COLLECTOR>
void coSBVH::FindOverlaps(COLLECTOR collector, const coSphere& sphere) const
{
	const coFloatx4 radius = coBroadcastW(sphere.centerAndRadius);
	const coAabb aabb(coVec3(sphere.centerAndRadius) - radius, coVec3(sphere.centerAndRadius) + radius);
	coUint triangleIndices[MAX_NB];
	coUint nb = 0;
	auto collector2 = [collector, &triangleIndices, &nb](const coUint32 triangleIdx)
	{
		if (nb == MAX_NB)
			return;
		for (coUint i = 0; i < nb; ++i)
		{
			if (triangleIndices[i] == triangleIdx)
				return;
		}
		triangleIndices[nb++] = triangleIdx;
		collector(triangleIdx);
	};
	FindOverlapsImpl(collector2, aabb);
}