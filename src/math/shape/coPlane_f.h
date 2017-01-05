// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/shape/coPlane.h"
#include "math/scalar/coFloat_f.h"
#include "math/vector/coFloatx3_f.h"
#include "math/vector/coFloatx4_f.h"
#include "debug/log/coAssert.h"

coFORCE_INLINE coBool32x3 coIsValid(const coPlane& _this)
{
	return coIsNormalized(reinterpret_cast<const coFloatx3&>(_this.normalAndDistance));
}

coFORCE_INLINE void coSetFromNormalAndSignedDistance(coPlane& _this, const coFloatx3& _normal, const coFloatx3& _dist)
{
	coASSERT(coAreXYZEqual(_dist));
	_this.normalAndDistance = coSelectXYZ(reinterpret_cast<const coFloatx4&>(_normal), -coBroadcastX(reinterpret_cast<const coFloatx4&>(_dist)));
	coASSERT(coIsValid(_this));
}

coFORCE_INLINE void coSetFromNormalAndPoint(coPlane& _this, const coFloatx3& _normal, const coFloatx3& _point)
{
	coSetFromNormalAndSignedDistance(_this, _normal, coDot(_normal, _point));
}

coFORCE_INLINE coFloatx4 coDistanceToPoint(const coPlane& _this, const coFloatx4& _point)
{
	coASSERT(_point.w == 1.0f);
	return coDot(_this.normalAndDistance, _point);
}

coFORCE_INLINE coFloatx3 coDistanceToPoint(const coPlane& _this, const coFloatx3& _point)
{
	return coDot(_this.normalAndDistance, coFloatx4(_point, 1.0f));
}

coFORCE_INLINE coBool32x4 coRayPlaneIntersection(const coPlane& _this, const coFloatx4& _rayPoint, const coFloatx4& _rayDir, coFloatx4& _t)
{
	coASSERT(_rayPoint.w == 1.0f);
	coASSERT(_rayDir.w == 0.0f);
	const coFloatx4 dot = coDot(_rayPoint, _this.normalAndDistance);
	const coFloatx4 dot2 = coDot(_rayDir, _this.normalAndDistance);
	if (coNotNearEqual0(dot2))
	{
		_t = - dot / dot2;
		return coBool32x4_true;
	}
	else
	{
		_t = 0.0f;
		return coBool32x4_false;
	}
}

coFORCE_INLINE coBool32x3 coRayPlaneIntersection(const coPlane& _this, const coFloatx3& _rayPoint, const coFloatx3& _rayDir, coFloatx3& _t)
{
	return coRayPlaneIntersection(_this, coFloatx4(_rayPoint, 1.0f), coFloatx4(_rayDir, 0.0f), reinterpret_cast<coFloatx4&>(_t));
}
