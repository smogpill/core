// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/shape/coPlane.h"
#include "math/shape/coRay.h"
#include "math/scalar/coFloat_f.h"
#include "math/vector/coVec3_f.h"
#include "math/vector/coVec4_f.h"
#include "debug/log/coAssert.h"

coFORCE_INLINE coBool coIsValid(const coPlane& this_)
{
	return coAreAllTrue(coIsNormalized(coBitCast<coVec3>(this_.normalAndDistance)));
}

coFORCE_INLINE void coSetFromNormalAndSignedDistance(coPlane& this_, const coVec3& normal, const coFloatx4& dist)
{
	coASSERT(coAreAllTrue(coAreXYZWEqual(dist)));
	this_.normalAndDistance = coSelectXYZ(-coSplatX(dist), reinterpret_cast<const coFloatx4&>(normal));
	coASSERT(coIsValid(this_));
}

coFORCE_INLINE coFloatx4 coDistance(const coPlane& this_, const coVec4& point)
{
	coASSERT(point.w == 1.0f);
	return coDot(this_.normalAndDistance, point);
}

coFORCE_INLINE coFloatx4 coDistance(const coPlane& this_, const coVec3& point)
{
	return coDot(this_.normalAndDistance, coVec4(point, 1.0f));
}

coFORCE_INLINE coVec3 coProject(const coPlane& this_, const coVec3& point)
{
	return point - coDistance(this_, point) * coVec3(this_.normalAndDistance);
}

coFORCE_INLINE coBool32x4 coRayPlaneIntersection(const coPlane& this_, const coVec4& rayPoint, const coVec4& rayDir, coFloatx4& t)
{
	coASSERT(rayPoint.w == 1.0f);
	coASSERT(rayDir.w == 0.0f);
	const coFloatx4 dot = coDot(rayPoint, this_.normalAndDistance);
	const coFloatx4 dot2 = coDot(rayDir, this_.normalAndDistance);
	if (coAreAllTrue(coNotNearEqual0(dot2)))
	{
		t = - dot / dot2;
		return coBool32x4_true;
	}
	else
	{
		t = 0.0f;
		return coBool32x4_false;
	}
}

coFORCE_INLINE coBool32x3 coRayPlaneIntersection(const coPlane& this_, const coInfiniteRay& ray, coFloatx3& t)
{
	return coRayPlaneIntersection(this_, coFloatx4(ray.origin, 1.0f), coFloatx4(ray.GetDir(), 0.0f), reinterpret_cast<coFloatx4&>(t));
}
