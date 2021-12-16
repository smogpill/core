// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coIntersectTriangleBox_f.h"
#include <math/vector/coVec3_f.h>
#include <math/vector/coUint32x4.h>

static const coUint32x4 signV = coUint32x4(0x80000000, 0x80000000, 0x80000000, 0x80000000);

static coFORCE_INLINE coInt coTestClassIIIAxes(const coVec3& e0V, const coVec3 v0V, const coVec3 v1V, const coVec3 v2V, const coVec3& extents)
{
	/*
	const Vec4V e0XZY_V = V4PermYZXW(e0V);

	const Vec4V v0XZY_V = V4PermYZXW(v0V);
	const Vec4V p0V = V4NegMulSub(v0XZY_V, e0V, V4Mul(v0V, e0XZY_V));

	const Vec4V v1XZY_V = V4PermYZXW(v1V);
	const Vec4V p1V = V4NegMulSub(v1XZY_V, e0V, V4Mul(v1V, e0XZY_V));

	const Vec4V v2XZY_V = V4PermYZXW(v2V);
	const Vec4V p2V = V4NegMulSub(v2XZY_V, e0V, V4Mul(v2V, e0XZY_V));

	Vec4V minV = V4Min(p0V, p1V);
	minV = V4Min(minV, p2V);

	const Vec4V extentsV = V4LoadU(&extents.x);
	const Vec4V fe0ZYX_V = V4Abs(e0V);

	const Vec4V fe0XZY_V = V4PermYZXW(fe0ZYX_V);
	const Vec4V extentsXZY_V = V4PermYZXW(extentsV);
	Vec4V radV = V4MulAdd(extentsV, fe0XZY_V, V4Mul(extentsXZY_V, fe0ZYX_V));

	if (V4AnyGrtr3(minV, radV))
		return 0;

	Vec4V maxV = V4Max(p0V, p1V);
	maxV = V4Max(maxV, p2V);

	radV = V4Sub(V4Zero(), radV);

	if (V4AnyGrtr3(radV, maxV))
		return 0;
	return 1;
	*/
	return 0;
}

static coFORCE_INLINE coInt _coIntersectTriangleBoxInternal(const coVec3 v0V, const coVec3 v1V, const coVec3 v2V, const coVec3& extents)
{
	/*
	// Test box axes
	{
		coVec3 extentsV = extents;

		{
			const coVec3 cV = coAbs(v0V);
			if (coAreAllTrue(extentsV >= cV))
				return 1;
		}

		coVec3 minV = coMin(v0V, v1V);
		minV = coMin(minV, v2V);

		if (V4AnyGrtr3(minV, extentsV))
			return 0;

		coVec3 maxV = coMax(v0V, v1V);
		maxV = coMax(maxV, v2V);
		extentsV = -extentsV;

		if (V4AnyGrtr3(extentsV, maxV))
			return 0;
	}

	// Test if the box intersects the plane of the triangle
	const coVec3 e0V = v1V - v0V;
	const coVec3 e1V = v2V - v1V;
	{
		const coVec3 normalV = coCross(e0V, e1V);
		const coVec3 dV = Vec4V_From_FloatV(coDot(normalV, v0V));

		const coVec3 extentsV = extents;
		VecU32V normalSignsV = V4U32and(VecU32V_ReinterpretFrom_Vec4V(normalV), signV);
		const coVec3 maxV = Vec4V_ReinterpretFrom_VecU32V(V4U32or(VecU32V_ReinterpretFrom_Vec4V(extentsV), normalSignsV));

		coVec3 tmpV = Vec4V_From_FloatV(V4Dot3(normalV, maxV));
		if (V4AnyGrtr3(dV, tmpV))
			return 0;

		normalSignsV = V4U32xor(normalSignsV, signV);
		const coVec3 minV = Vec4V_ReinterpretFrom_VecU32V(V4U32or(VecU32V_ReinterpretFrom_Vec4V(extentsV), normalSignsV));

		tmpV = Vec4V_From_FloatV(coDot(normalV, minV));
		if (V4AnyGrtr3(tmpV, dV))
			return 0;
	}

	// Edge-edge tests
	{
		if (!coTestClassIIIAxes(e0V, v0V, v1V, v2V, extents))
			return 0;
		if (!coTestClassIIIAxes(e1V, v0V, v1V, v2V, extents))
			return 0;
		const coVec3 e2V = v0V - v2V;
		if (!coTestClassIIIAxes(e2V, v0V, v1V, v2V, extents))
			return 0;
	}
	return 1;
	*/
	return 0;
}

/*
coInt coIntersectTriangleBox(const BoxPadded& box, const coVec3& p0_, const coVec3& p1_, const coVec3& p2_)
{
	// PT: TODO: SIMDify this part

	// Vec3p ensures we can safely V4LoadU the data
	const coVec3 p0 = box.rotateInv(p0_ - box.center);
	const coVec3 p1 = box.rotateInv(p1_ - box.center);
	const coVec3 p2 = box.rotateInv(p2_ - box.center);

	return _coIntersectTriangleBoxInternal(p0_, p1_, p2_, box.extents);
}
*/
