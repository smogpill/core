// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coSBVH.h"
#include "container/array/coDynamicArray_f.h"
#include "math/shape/coAabb_f.h"
#include "math/shape/coRay.h"
#include "math/shape/coOverlap_f.h"

// Implementation of Spatial Splits in Bounding Volume Hierarchies, Martin Stich, Heiko Friedrich and Andreas Dietrich from NVIDIA
// https://www.nvidia.in/docs/IO/77714/sbvh.pdf

void coSBVH::Build(const coArray<coVec3>& vertices_, const coArray<coUint32>& indices_)
{
	coClear(nodes);
	vertices = &vertices_;
	indices = &indices_;

	coASSERT(indices_.count % 3 == 0);
	const coUint32 nbTriangles = indices_.count / 3;

	// Compute triangle AABBs
	{
		coClear(aabbs);
		coResize(aabbs, nbTriangles);
		coUint32 index = 0;
		for (coUint32 triangleIdx = 0; triangleIdx < nbTriangles; ++triangleIdx)
		{
			const coUint32 aIdx = indices_[index++];
			const coUint32 bIdx = indices_[index++];
			const coUint32 cIdx = indices_[index++];
			const coVec3 a = vertices_[aIdx];
			const coVec3 b = vertices_[bIdx];
			const coVec3 c = vertices_[cIdx];
			coAabb& aabb = aabbs[triangleIdx];
			aabb = coMerge(aabb, a);
			aabb = coMerge(aabb, b);
			aabb = coMerge(aabb, c);
		}
	}

	coDynamicArray<Ref> refsTemp;
	coReserve(refsTemp, nbTriangles * 2);
	coResize(refsTemp, nbTriangles);
	for (coUint32 triangleIdx = 0; triangleIdx < nbTriangles; ++triangleIdx)
	{
		Ref& ref = refsTemp[triangleIdx];
		ref.aabb = aabbs[triangleIdx];
		ref.triangle = triangleIdx;
	}

	BuildRec(refsTemp, aabbs, 0);
}

void coSBVH::Split(coAabb* left_, coAabb* right_, const coUint32 triangleIdx_, coUint8 axis_, coFloat pos_)
{
	// Impl based on: https://github.com/lighttransport/nanort/blob/sbvh/nanort.h (MIT license)

	coClear(*left_);
	coClear(*right_);

	coASSERT(triangleIdx_ != static_cast<coUint32>(-1));
	const coUint32 aIdx = (*indices)[3 * triangleIdx_ + 0];
	const coUint32 bIdx = (*indices)[3 * triangleIdx_ + 1];
	const coUint32 cIdx = (*indices)[3 * triangleIdx_ + 2];

	coVec3 v[3];
	v[0] = (*vertices)[aIdx];
	v[1] = (*vertices)[bIdx];
	v[2] = (*vertices)[cIdx];

	// Clip triangle to left and right aabb by processing all edges
	coAabb leftAabb, rightAabb;
	coVec3 v1 = v[2];
	for (coUint32 i = 0; i < 3; i++)
	{
		coVec3 v0 = v1;
		v1 = v[i];
		coFloat fV0d = v0[axis_], v1d = v1[axis_];

		if (fV0d <= pos_)
		{
			leftAabb = coMerge(leftAabb, v0); // This point is on left side
		}
		if (fV0d >= pos_)
		{
			rightAabb = coMerge(rightAabb, v0); // This point is on right side
		}

		if ((fV0d < pos_ && pos_ < v1d) || (v1d < pos_ && pos_ < fV0d))
		{
			// The edge crosses the splitting location
			coASSERT((v1d - fV0d) != 0.0f);
			coVec3 vC = v0 + (pos_ - fV0d) / (v1d - fV0d) * (v1 - v0);
			leftAabb = coMerge(leftAabb, vC);
			rightAabb = coMerge(rightAabb, vC);
		}
	}

	// Clip against current bounds
	const coAabb& bounds = aabbs[triangleIdx_];

	(*left_) = coIntersect(leftAabb, bounds);
	(*right_) = coIntersect(rightAabb, bounds);
}

coUint32 coSBVH::BuildRec(const coArray<Ref>& refs_, const coArray<coAabb>& primitives_, coUint32 depth_)
{
	// Impl based on: https://github.com/lighttransport/nanort/blob/sbvh/nanort.h (MIT license)

	const coUint32 nbPrimitives = refs_.count;
	coASSERT(nbPrimitives >= 0);

	const coUint32 nodesOffset = nodes.count;
	const coUint32 n = nbPrimitives;

	coAabb globalAABB;
	for (const Ref& ref : refs_)
	{
		globalAABB = coMerge(globalAABB, ref.aabb);
	}

	if (n <= 1 || depth_ == maxDepth)
	{
		// Emit refs
		coUint32 uiRefsOffset = refs.count;
		for (coUint32 i = 0; i < n; ++i)
		{
			coPushBack(refs, refs_[i]);
		}

		// Create leaf node.
		Node leaf;
		leaf.aabb = globalAABB;
		leaf.leaf = true;
		leaf.datas[0] = n;
		leaf.datas[1] = uiRefsOffset;
		coPushBack(nodes, leaf);

		return nodesOffset;
	}

	// Compute the AABB of the centers
	coAabb centerAABB;
	for (const Ref& ref : refs_)
	{
		const coVec3 center = coGetCenter(ref.aabb);
		centerAABB = coMerge(centerAABB, center);
	}

	const coVec3 centerAABBSize = coGetSize(centerAABB);

	// Choose the Split axis
	coUint8 splitAxis = 0;
	{
		coFloat max = centerAABBSize.x;
		if (centerAABBSize.y > max)
		{
			splitAxis = 1;
			max = centerAABBSize.y;
		}
		if (centerAABBSize.z > max)
		{
			splitAxis = 2;
		}
	}

	// Split pos
	const coFloat splitPos = coGetCenter(centerAABB)[splitAxis];

	enum Flag
	{
		DISJOINT_LEFT = 1 << 1,
		DISJOINT_RIGHT = 1 << 2,
		OVERLAPPED_LEFT = 1 << 3,
		OVERLAPPED_RIGHT = 1 << 4
	};

	// Classification
	coBool split = false;
	coDynamicArray<Ref> leftSplitRefs;
	coDynamicArray<Ref> rightSplitRefs;
	{
		coAabb D_L, D_R;
		coAabb O_L, O_R;
		coUint32 nbLeftDisjointTriangles = 0;
		coUint32 nbRightDisjointTriangles = 0;
		coUint32 nbLeftOverlapTriangles = 0;
		coUint32 nbRightOverlapTriangles = 0;
		for (coUint32 i = 0; i < n; i++)
		{
			refs_[i].flags = 0;
			if (refs_[i].aabb.max[splitAxis] < splitPos)
			{
				// D_L: Disjoint left
				refs_[i].flags |= DISJOINT_LEFT;
				D_L = coMerge(D_L, refs_[i].aabb);
				nbLeftDisjointTriangles++;
			}
			else if (refs_[i].aabb.min[splitAxis] > splitPos)
			{
				// D_R: Disjoint right
				refs_[i].flags |= DISJOINT_RIGHT;
				D_R = coMerge(D_R, refs_[i].aabb);
				nbRightDisjointTriangles++;
			}
			else
			{
				// Overlapping
				const coFloat bcenter = 0.5f * (refs_[i].aabb.min[splitAxis] + refs_[i].aabb.max[splitAxis]);
				if (bcenter < splitPos)
				{
					refs_[i].flags |= OVERLAPPED_LEFT;
					O_L = coMerge(O_L, refs_[i].aabb);
					nbLeftOverlapTriangles++;
				}
				else
				{
					refs_[i].flags |= OVERLAPPED_RIGHT;
					O_R = coMerge(O_R, refs_[i].aabb);
					nbRightOverlapTriangles++;
				}
			}
		}

		// Split O_L and O_R primitive to compute S_L and S_R.
		coAabb S_L, S_R;
		for (coUint32 i = 0; i < nbPrimitives; ++i)
		{
			if ((refs_[i].flags & OVERLAPPED_LEFT) || (refs_[i].flags & OVERLAPPED_RIGHT))
			{
				coAabb left_bbox, right_bbox;
				Split(&left_bbox, &right_bbox, refs_[i].triangle, splitAxis, splitPos);

				Ref left_prim_ref, right_prim_ref;

				left_prim_ref.aabb = left_bbox;
				left_prim_ref.triangle = refs_[i].triangle;
				left_prim_ref.flags = 0; // Clear `flag` with 0 for just in case.

				right_prim_ref.aabb = right_bbox;
				right_prim_ref.triangle = refs_[i].triangle;
				right_prim_ref.flags = 0;

				// Update the bound for a split set
				S_L = coMerge(S_L, left_bbox);
				S_R = coMerge(S_R, right_bbox);

				coPushBack(leftSplitRefs, left_prim_ref);
				coPushBack(rightSplitRefs, right_prim_ref);
			}
		}

		// Compute SAH cost.
		//
		// C_O = A(Union(D_L, O_L)) |Union(D_L, O_L)| + A(Union(D_R, O_R))|Union(D_R, O_R)|
		// C_S = A(Union(D_L, S_L)) |Union(D_L, S_L)| + A(Union(D_R, S_R))|Union(D_R, S_R)|
		// where A is the surface area of a set, C_O is the SAH cost of keeping the
		// original triangles, and C_S is the SAH cost of using the split primitives.
		// SAH cost when using the split sets

		coFloat C_O = 0.0f;
		coFloat C_S = 0.0f;

		// |Union(D, O)| = # of prims in D + # of prims in O, since there is no overlap in Disjoint set and Overlapping set.
		// |Union(D, S|| = # of prims in D + # of prims in S, since there is no overlap in Disjoint set and Split set.
		// SurfaceArea(Union(a, b)) = SurfaceArea(a) + SurfaceArea(b) - SurfaceArea(Intersect(a, b))
		const coAabb I_D_L_O_L = coIntersect(D_L, O_L);
		const coAabb I_D_R_O_R = coIntersect(D_R, O_R);
		const coAabb I_D_L_S_L = coIntersect(D_L, S_L);
		const coAabb I_D_R_S_R = coIntersect(D_R, S_R);

		const coUint32 uiN_D_L_O_L = nbLeftDisjointTriangles + nbLeftOverlapTriangles;
		const coUint32 uiN_D_R_O_R = nbRightDisjointTriangles + nbRightOverlapTriangles;

		// #(S_L) = #(S_R) = #(O_L) + #(O_R)
		const coUint32 uiN_D_L_S_L = nbLeftDisjointTriangles + nbLeftOverlapTriangles + nbRightOverlapTriangles;
		const coUint32 uiN_D_R_S_R = nbRightDisjointTriangles + nbLeftOverlapTriangles + nbRightOverlapTriangles;

		const coFloat fSA_D_L_O_L = coGetSurfaceArea(D_L) + coGetSurfaceArea(O_L) - coGetSurfaceArea(I_D_L_O_L);
		const coFloat fSA_D_R_O_R = coGetSurfaceArea(D_R) + coGetSurfaceArea(O_R) - coGetSurfaceArea(I_D_R_O_R);
		const coFloat fSA_D_L_S_L = coGetSurfaceArea(D_L) + coGetSurfaceArea(S_L) - coGetSurfaceArea(I_D_L_S_L);
		const coFloat fSA_D_R_S_R = coGetSurfaceArea(D_R) + coGetSurfaceArea(S_R) - coGetSurfaceArea(I_D_R_S_R);

		coASSERT(coIsValid(fSA_D_L_O_L));
		coASSERT(coIsValid(fSA_D_R_O_R));
		coASSERT(coIsValid(fSA_D_L_S_L));
		coASSERT(coIsValid(fSA_D_R_S_R));

		C_O = fSA_D_L_O_L * uiN_D_L_O_L + fSA_D_R_O_R * uiN_D_R_O_R;
		C_S = fSA_D_L_S_L * uiN_D_L_S_L + fSA_D_R_S_R * uiN_D_R_S_R;

		coASSERT(coIsValid(C_O));
		coASSERT(coIsValid(C_S));

		split = C_S < C_O;
	}

	Node node;
	node.leaf = false;

	coPushBack(nodes, node);

	coDynamicArray<Ref> leftRefs;
	coDynamicArray<Ref> rightRefs;
	coReserve(leftRefs, refs_.count * 2);
	coReserve(rightRefs, refs_.count * 2);

	if (split)
	{
		// New left/right set = Disjoint set + Split set
		for (coUint32 i = 0; i < nbPrimitives; i++)
		{
			if (refs_[i].flags & DISJOINT_LEFT)
			{
				coPushBack(leftRefs, refs_[i]);
			}
			else if (refs_[i].flags & DISJOINT_RIGHT)
			{
				coPushBack(rightRefs, refs_[i]);
			}
		}

		coPushBackArray(leftRefs, leftSplitRefs);
		coPushBackArray(rightRefs, rightSplitRefs);
	}
	else
	{
		// New left/right set = Disjoint set + Overlapping set
		for (coUint32 i = 0; i < nbPrimitives; ++i)
		{
			if ((refs_[i].flags & DISJOINT_LEFT) || (refs_[i].flags & OVERLAPPED_LEFT))
			{
				coPushBack(leftRefs, refs_[i]);
			}
			else if ((refs_[i].flags & DISJOINT_RIGHT) || (refs_[i].flags & OVERLAPPED_RIGHT))
			{
				coPushBack(rightRefs, refs_[i]);
			}
		}

		// Use object median for an corner case.
		if (leftRefs.count == 0)
		{
			const coUint32 halfNb = rightRefs.count / 2;
			for (coUint32 i = 0; i < halfNb; ++i)
			{
				coPushBack(leftRefs, coBack(rightRefs));
				coPopBack(rightRefs);
			}
		}
		else if (rightRefs.count == 0)
		{
			const coUint32 halfNb = leftRefs.count / 2;
			for (coUint32 i = 0; i < halfNb; ++i)
			{
				coPushBack(rightRefs, coBack(leftRefs));
				coPopBack(leftRefs);
			}
		}
	}

	const coUint32 leftChildNodeIndex = BuildRec(leftRefs, primitives_, depth_ + 1);
	const coUint32 rightChildNodeIndex = BuildRec(rightRefs, primitives_, depth_ + 1);

	Node& outNode = nodes[nodesOffset];
	outNode.datas[0] = leftChildNodeIndex;
	outNode.datas[1] = rightChildNodeIndex;
	outNode.aabb = globalAABB;

	return nodesOffset;
}

void coSBVH::FindOverlaps(coDynamicArray<coUint32>& triangles_, const coAabb& aabb_) const
{
	FindOverlapsImpl(triangles_, aabb_);
}

void coSBVH::FindOverlaps(coDynamicArray<coUint32>& triangles_, const coRay& ray_)const
{
	Ray2 ray2;
	ray2.origin = ray_.origin;
	// We add some epsilon because Recipral seems to create NaN instead of inf with the 0 components of the direction.
	ray2.invDir = coInv(ray_.GetDir() + coVec3(1e-20f));
	coASSERT(coIsValid(ray2.invDir));
 	ray2.len = coBroadcastW(ray_.dirAndLength);

	FindOverlapsImpl(triangles_, ray2);
}

void coSBVH::FindOverlaps(coDynamicArray<coUint32>& triangles_, const coVec3& halfSize_, const coRay& ray_) const
{
	MovingAABB movingAabb;
	movingAabb.ray.origin = ray_.origin;
	// We add some epsilon because Recipral seems to create NaN instead of inf with the 0 components of the direction.
	movingAabb.ray.invDir = coInv(ray_.GetDir() + coVec3(1e-20f));
	coASSERT(coIsValid(movingAabb.ray.invDir));
	movingAabb.ray.len = coBroadcastW(ray_.dirAndLength);
	movingAabb.halfSize = halfSize_;

	FindOverlapsImpl(triangles_, movingAabb);
}

template <class T>
coFORCE_INLINE void coSBVH::FindOverlapsImpl(coDynamicArray<coUint32>& triangles_, const T& query) const
{
	coClear(triangles_);
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
				if (!coContains(triangles_, ref.triangle))
					coPushBack(triangles_, ref.triangle);
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
