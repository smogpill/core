// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coRay.h"
#include "../vector/coVec3_f.h"
#include "math/transform/coTransform_f.h"
#include "math/matrix/coMat4_f.h"

coFORCE_INLINE coFloatx4 coSquareDistance(const coInfiniteRay& ray, const coVec3& point)
{
	return coSquareLength(coCross(ray.GetDir(), point - ray.origin));
}

coFORCE_INLINE coFloatx4 coDistance(const coInfiniteRay& ray, const coVec3& point)
{
	return coLength(coCross(ray.GetDir(), point - ray.origin));
}

coFORCE_INLINE coRay coInverseTransform(const coTransform& t, const coRay& ray)
{
	coRay out(nullptr);
	coMat4 m(t);
	coMat4 inv = coInverse(m);
	out.origin = coTransformPosition(inv, ray.origin);
	out.dirAndLength = coVec4(coNormalize(coTransformVector(inv, coVec3(ray.dirAndLength))), ray.dirAndLength.w / t.scale.z);
	//out.origin = coInverseTransformPosition(t, ray.origin);
	//out.dirAndLength = coVec4(coInverseTransformVector(t, coVec3(ray.dirAndLength)), ray.dirAndLength.w / t.scale.z);
	return out;
}
