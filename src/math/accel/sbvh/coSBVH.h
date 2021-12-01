// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray.h>
#include <math/vector/coVec3.h>
#include <math/vector/coFloatx4.h>
#include <math/shape/coAabb.h>
class coRay;

class coSBVH
{
public:
	void Build(const coArray<coVec3>& vertices, const coArray<coUint32>& indices);
	void FindOverlaps(coDynamicArray<coUint32>& outTriangles, const coAabb& aabb) const;
	void FindOverlaps(coDynamicArray<coUint32>& outTriangles, const coVec3& halfSize, const coRay& ray) const;
	void FindOverlaps(coDynamicArray<coUint32>& outTriangles, const coRay& ray) const;

private:
	struct Node
	{
		coAabb aabb;
		coBool leaf = false;
		coUint8 splitAxis = 0;
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
	template <class T>
	void FindOverlapsImpl(coDynamicArray<coUint32>& outTriangles, const T& query) const;
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
};
