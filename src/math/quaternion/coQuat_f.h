// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/scalar/coFloat_f.h"
#include "math/vector/coVec3_f.h"
#include "math/vector/coVec4_f.h"
#include "math/vector/coInt32x4_f.h"
#include "math/quaternion/coQuat.h"
#include "lang/coCppExtensions.h"
#include "debug/log/coLog.h"

coFORCE_INLINE coQuat coNormalize(const coQuat& _this)
{
	return coQuat(coNormalize(static_cast<const coVec4&>(_this)));
}
coFORCE_INLINE coBool32x4 coIsNormalized(const coQuat& _this, const coVec4& _squareEpsilon = coVec4(1e-3f))
{
	return coIsNormalized(coBitCast<coVec4>(_this), _squareEpsilon);
}

coFORCE_INLINE coQuat coRotation(const coFloatx3& _eulerAngles)
{
	// Uses conventions from Unreal from FRotator::Quaternion()

	/*
	const float DEG_TO_RAD = PI/(180.f);
	const float RADS_DIVIDED_BY_2 = DEG_TO_RAD/2.f;
	float SP, SY, SR;
	float CP, CY, CR;

	const float PitchNoWinding = FMath::Fmod(Pitch, 360.0f);
	const float YawNoWinding = FMath::Fmod(Yaw, 360.0f);
	const float RollNoWinding = FMath::Fmod(Roll, 360.0f);

	FMath::SinCos(&SP, &CP, PitchNoWinding * RADS_DIVIDED_BY_2);
	FMath::SinCos(&SY, &CY, YawNoWinding * RADS_DIVIDED_BY_2);
	FMath::SinCos(&SR, &CR, RollNoWinding * RADS_DIVIDED_BY_2);

	FQuat RotationQuat;
	RotationQuat.X =  CR*SP*SY - SR*CP*CY;
	RotationQuat.Y = -CR*SP*CY - SR*CP*SY;
	RotationQuat.Z =  CR*CP*SY - SR*SP*CY;
	RotationQuat.W =  CR*CP*CY + SR*SP*SY;*/

	const coFloatx3 halfAngles = _eulerAngles * 0.5f;
	const coFloatx3 s = coSin(halfAngles);
	const coFloatx3 c = coCos(halfAngles);

	const coFloat sp = s.y;
	const coFloat cp = c.y;
	const coFloat sy = s.z;
	const coFloat cy = c.z;
	const coFloat sr = s.x;
	const coFloat cr = c.x;

	coQuat r(nullptr);
	r.x = +cr*sp*sy - sr*cp*cy;
	r.y = -cr*sp*cy - sr*cp*sy;
	r.z = +cr*cp*sy - sr*sp*cy;
	r.w = +cr*cp*cy + sr*sp*sy;

	r  = coNormalize(r); // HACK
	return r;
}

coFORCE_INLINE coQuat coRotation(const coVec3& _axis, coFloat _angle)
{
	coASSERT(coIsNormalized(_axis));
	const coFloatx4 halfAngle = _angle * 0.5f;
	const coFloatx4 s = coSin(halfAngle);
	const coFloatx4 c = coCos(halfAngle);
	const coQuat q = coBitCast<coQuat>(coSelectXYZ(coBitCast<coFloatx4>(_axis) * s, c));
	return q;
}
coFORCE_INLINE coQuat _coRotation(const coVec3& _from, const coVec3& _to, coFloatx3 _fromToNorm)
{
	// http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/index.htm
	// http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/minorlogic.htm
	const coFloatx3 w = _fromToNorm + coDot(_from, _to);
	if (w > 1e-6f * _fromToNorm)
	{
		const coFloatx3 axis = coCross(_from, _to);
		const coFloatx4 q = coSelectXYZ(coBitCast<coFloatx4>(axis), coBroadcastX(coBitCast<coFloatx4>(w)));
		return coNormalize(coBitCast<coQuat>(q));
	}
	else
	{
		//return coNormalize(coAbs(_from.x) > coAbs(_from.y) ? coQuat(-_from.z, 0.0f, _from.x, 0.0f) : coQuat(0.0f, -_from.z, _from.y, 0.0f));
		return coNormalize(coQuat(-_from.z, _from.y, _from.x, 0.0f));
	}
}
coFORCE_INLINE coQuat coRotationUnitVecs(const coVec3& _from, const coVec3& _to)
{
	coASSERT(coIsNormalized(_from));
	coASSERT(coIsNormalized(_to));
	return _coRotation(_from, _to, 1.0f);
}
coFORCE_INLINE coQuat coRotation(const coVec3& _from, const coVec3& _to)
{
	return _coRotation(_from, _to, coSquareRoot(coSquareLength(_from) * coSquareLength(_to)));
}
coFORCE_INLINE coQuat coInverse(const coQuat& _a) { return coBitCast<coQuat>(coBitCast<coInt32x4>(_a) ^ coInt32x4_SIGN_MASK_XYZ); }
coFORCE_INLINE coQuat coExp(const coQuat& _a)
{
	const float r = coSquareRoot(_a.x*_a.x + _a.y*_a.y + _a.z*_a.z);
	const float expW = coExp(_a.w);
	const float s = (r >= 0.00001f) ? expW * coSin(r) / r : 0.0f;
	return coQuat(s*_a.x, s*_a.y, s*_a.z, expW * coCos(r));
}
coFORCE_INLINE coQuat coLog(const coQuat& _a)
{
	coASSERT(false); // Check if the coAtan has the correct order of parameters.
	const float r = coSquareRoot(_a.x*_a.x + _a.y*_a.y + _a.z*_a.z);
	const float t = (r > 0.00001f) ? coAtan(r, _a.w) / r : 0.0f;
	return coQuat(t*_a.x, t*_a.y, t*_a.z, 0.5f * coLog(_a.x*_a.x + _a.y*_a.y + _a.z*_a.z + _a.w*_a.w));
}
coFORCE_INLINE coQuat coSyncWith(const coQuat& _a, const coQuat& _b)
{
	const coInt32x4 comp = coBitCast<coInt32x4>(coDot(_a, _b) < coFloatx4_ZERO);
	return coBitCast<coQuat>(coBitCast<coInt32x4>(_a) ^ (comp & coBitCast<coInt32x4>(__m128_SIGN_MASK)));
}
coFORCE_INLINE coQuat operator*(const coQuat& _a, const coQuat& _b)
{
	// Implementation found at: https://github.com/bulletphysics/bullet3/blob/master/src/Bullet3Common/b3Quaternion.h
	const coFloatx4 a = coBitCast<coFloatx4>(_a);
	const coFloatx4 b = coBitCast<coFloatx4>(_b);
	coFloatx4 a1 = coShuffle<0, 1, 2, 0>(a);
	coFloatx4 b1 = coShuffle<3, 3, 3, 0>(b);
	a1 = a1 * b1;
	coFloatx4 a2 = coShuffle<1, 2, 0, 1>(a);
	coFloatx4 b2 = coShuffle<2, 0, 1, 1>(a);
	a2 = a2 * b2;
	b1 = coShuffle<2, 0, 1, 2>(a);
	b2 = coShuffle<1, 2, 0, 2>(b);
	b1 = b1 * b2;
	coFloatx4 a0 = coBroadcastW(a);
	a0 = a0 * b;
	a1 = a1 + a2;
	a0 = a0 - b1;
	a1 = coBitCast<coFloatx4>(coBitCast<coInt32x4>(a1) ^ coInt32x4_SIGN_MASK_W);
	a0 = a0 + a1;
	return coBitCast<coQuat>(a0);
}
coFORCE_INLINE coQuat& operator*=(coQuat& this_, const coQuat& b)
{
	return this_ = this_ * b;
}
// coFORCE_INLINE coQuat operator*(const coQuat& _q, const coFloatx4& _xyzw)
// {
// 	coASSERT(false);
// 	return coMake_quat(coBitCast<coFloatx4>(_q) + _xyzw);
// }
coFORCE_INLINE coBool32x4 coIsValid(const coQuat& _this) { return coIsValid(coBitCast<coFloatx4>(_this)); }
void coSetupSquad(coQuat& _out0, coQuat& _out1, coQuat& _out3, const coQuat& _q0, const coQuat& _q1, const coQuat& _q2, const coQuat& _q3);
coFORCE_INLINE coVec3 coRotateVector(const coQuat& _this, const coVec3& _vec)
{
	// https://blog.molecular-matters.com/2013/05/24/a-faster-quaternion-vector-multiplication/

	const coVec3 q = coBitCast<coVec3>(_this);
	const coFloatx3 w = coFloatx3(coBroadcastW(_this));
	//return 2.0f * coDot(q, _vec) * q + (w*w - coDot(q, q)) * _vec + 2.0f * w * coCross(q, _vec);
	const coVec3 t = 2.0f * coCross(q, _vec);
	return _vec + w * t + coCross(q, t);
}

coFORCE_INLINE coVec3 coInverseRotateVector(const coQuat& _this, const coVec3& _vec)
{
	return coRotateVector(coInverse(_this), _vec);
}
