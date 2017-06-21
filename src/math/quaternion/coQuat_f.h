// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/scalar/coFloat_f.h"
#include "math/vector/coFloatx3_f.h"
#include "math/vector/coFloatx4_f.h"
#include "math/vector/coInt32x4_f.h"
#include "math/quaternion/coQuat.h"
#include "lang/coCppExtensions.h"
#include "debug/log/coLog.h"

coFORCE_INLINE const coFloatx4& coMake_floatx4(const coQuat& _a) { return coBitCast<coFloatx4>(_a); }
coFORCE_INLINE const coQuat& coMake_quat(const coFloatx4& _a) { return coBitCast<coQuat>(_a); }
coFORCE_INLINE coQuat coMake_quat(float _x, float _y, float _z, float _w) { return coMake_quat(coMake_floatx4( _x, _y, _z, _w )); }
coFORCE_INLINE coQuat coNormalize(const coQuat& _this)
{
	return coBitCast<coQuat>(coNormalize(coBitCast<coFloatx4>(_this)));
}
coFORCE_INLINE coQuat coRotation(const coFloatx3& _eulerAngles)
{
	const coFloatx3 halfAngles = _eulerAngles * 0.5f;
	const coFloatx3 s = coSin(halfAngles);
	const coFloatx3 c = coCos(halfAngles);
	const coFloat sp = s.x;
	const coFloat cp = c.x;
	const coFloat sy = s.y;
	const coFloat cy = c.y;
	const coFloat sr = s.z;
	const coFloat cr = c.z;
	coQuat r(nullptr);
// 	r.x = +cr*sp*sy - sr*cp*cy;
// 	r.y = -cr*sp*cy - sr*cp*sy;
// 	r.z = +cr*cp*sy - sr*sp*cy;
// 	r.w = +cr*cp*cy + sr*sp*sy;
	r.x = cr * sp * cy + sr * cp * sy;
	r.y = cr * cp * sy - sr * sp * cy;
	r.z = sr * cp * cy - cr * sp * sy;
	r.w = cr * cp * cy + sr * sp * sy;
	return r;
}
coFORCE_INLINE coQuat coRotation(const coFloatx3& _axis, coFloat _angle)
{
	coASSERT(coIsNormalized(_axis));
	const coFloatx4 halfAngle = _angle * 0.5f;
	const coFloatx4 s = coSin(halfAngle);
	const coFloatx4 c = coCos(halfAngle);
	const coQuat q = coBitCast<coQuat>(coSelectXYZ(coBitCast<coFloatx4>(_axis) * s, c));
	return q;
}
coFORCE_INLINE coQuat _coRotation(const coFloatx3& _from, const coFloatx3& _to, coFloatx3 _fromToNorm)
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
coFORCE_INLINE coQuat coRotationUnitVecs(const coFloatx3& _from, const coFloatx3& _to)
{
	coASSERT(coIsNormalized(_from));
	coASSERT(coIsNormalized(_to));
	return _coRotation(_from, _to, 1.0f);
}
coFORCE_INLINE coQuat coRotation(const coFloatx3& _from, const coFloatx3& _to)
{
	return _coRotation(_from, _to, coSqrt(coSquareLength(_from) * coSquareLength(_to)));
}
coFORCE_INLINE coQuat coInverse(const coQuat& _a) { return coBitCast<coQuat>(coBitCast<coInt32x4>(_a) ^ coInt32x4_SIGN_MASK_XYZ); }
coFORCE_INLINE coQuat coExp(const coQuat& _a)
{
	const float r = coSqrt(_a.x*_a.x + _a.y*_a.y + _a.z*_a.z);
	const float expW = coExp(_a.w);
	const float s = (r >= 0.00001f) ? expW * coSin(r) / r : 0.0f;
	return coMake_quat(s*_a.x, s*_a.y, s*_a.z, expW * coCos(r));
}
coFORCE_INLINE coQuat coLog(const coQuat& _a)
{
	const float r = coSqrt(_a.x*_a.x + _a.y*_a.y + _a.z*_a.z);
	const float t = (r > 0.00001f) ? coAtan(r, _a.w) / r : 0.0f;
	return coMake_quat(t*_a.x, t*_a.y, t*_a.z, 0.5f * coLog(_a.x*_a.x + _a.y*_a.y + _a.z*_a.z + _a.w*_a.w));
}
coFORCE_INLINE coQuat coSyncWith(const coQuat& _a, const coQuat& _b)
{
	const coInt32x4 comp = coBitCast<coInt32x4>(coDot(coMake_floatx4(_a), coMake_floatx4(_b)) < coFloatx4_ZERO);
	return coBitCast<coQuat>(coBitCast<coInt32x4>(_a) ^ (comp & coBitCast<coInt32x4>(__m128_SIGN_MASK)));
}
coFORCE_INLINE coBool32x4 coNearEqual(const coQuat& _a, const coQuat& _b, const coFloatx4& _epsilon = coFloatx4(1e-3f)) { return coNearEqual(coBitCast<coFloatx4>(_a), coBitCast<coFloatx4>(_b), _epsilon); }
coFORCE_INLINE coQuat operator*(const coQuat& _a, const coQuat& _b)
{
	// Implementation found at: https://github.com/bulletphysics/bullet3/blob/master/src/Bullet3Common/b3Quaternion.h
	const coFloatx4 a = coBitCast<coFloatx4>(_a);
	const coFloatx4 b = coBitCast<coFloatx4>(_b);
	coFloatx4 a1 = coSwizzle<0, 1, 2, 0>(a);
	coFloatx4 b1 = coSwizzle<3, 3, 3, 0>(b);
	a1 = a1 * b1;
	coFloatx4 a2 = coSwizzle<1, 2, 0, 1>(a);
	coFloatx4 b2 = coSwizzle<2, 0, 1, 1>(a);
	a2 = a2 * b2;
	b1 = coSwizzle<2, 0, 1, 2>(a);
	b2 = coSwizzle<1, 2, 0, 2>(b);
	b1 = b1 * b2;
	coFloatx4 a0 = coBroadcastW(a);
	a0 = a0 * b;
	a1 = a1 + a2;
	a0 = a0 - b1;
	a1 = coBitCast<coFloatx4>(coBitCast<coInt32x4>(a1) ^ coInt32x4_SIGN_MASK_W);
	a0 = a0 + a1;
	return coBitCast<coQuat>(a0);
// 	coFloatx4 res = coBroadcastW(a) * b;
// 	res += coBroadcastX(a) * coSwizzle<0, 1, 2, 3>(b);
// 	res += coBroadcastY(a) * coSwizzle<1, 0, 3, 2>(b);
// 	res += coBroadcastZ(a) * coSwizzle<2, 3, 0, 1>(b);
// 	return coBitCast<coQuat>(res);

// 	VectorRegister Result = VectorMultiply(VectorReplicate(Quat1, 3), Quat2);
// 	Result = VectorMultiplyAdd(VectorMultiply(VectorReplicate(Quat1, 0), VectorSwizzle(Quat2, 3, 2, 1, 0)), GlobalVectorConstants::QMULTI_SIGN_MASK0, Result);
// 	Result = VectorMultiplyAdd(VectorMultiply(VectorReplicate(Quat1, 1), VectorSwizzle(Quat2, 2, 3, 0, 1)), GlobalVectorConstants::QMULTI_SIGN_MASK1, Result);
// 	Result = VectorMultiplyAdd(VectorMultiply(VectorReplicate(Quat1, 2), VectorSwizzle(Quat2, 1, 0, 3, 2)), GlobalVectorConstants::QMULTI_SIGN_MASK2, Result);
//	return Result;

// 	A1 = b3_pshufd_ps(vQ1, B3_SHUFFLE(0, 1, 2, 0)); // X Y  z x     //      vtrn
// 	B1 = b3_pshufd_ps(vQ2, B3_SHUFFLE(3, 3, 3, 0)); // W W  W X     // vdup vext
// 	A1 = A1 * B1;
// 	A2 = b3_pshufd_ps(vQ1, B3_SHUFFLE(1, 2, 0, 1)); // Y Z  X Y     // vext 
// 	B2 = b3_pshufd_ps(vQ2, B3_SHUFFLE(2, 0, 1, 1)); // z x  Y Y     // vtrn vdup
// 	A2 = A2 * B2;
// 	B1 = b3_pshufd_ps(vQ1, B3_SHUFFLE(2, 0, 1, 2)); // z x Y Z      // vtrn vext
// 	B2 = b3_pshufd_ps(vQ2, B3_SHUFFLE(1, 2, 0, 2)); // Y Z x z      // vext vtrn
// 	B1 = B1 * B2;	//	A3 *= B3
// 	A0 = b3_splat_ps(vQ1, 3);	//	A0
// 	A0 = A0 * vQ2;	//	A0 * B0
// 	A1 = A1 + A2;	//	AB12
// 	A0 = A0 - B1;	//	AB03 = AB0 - AB3 
// 	A1 = _mm_xor_ps(A1, b3vPPPM);	//	change sign of the last element
// 	A0 = A0 + A1;	//	AB03 + AB12	
}
coFORCE_INLINE coBool32x4 operator== (const coQuat& _a, const coQuat& _b) { return coBitCast<coFloatx4>(_a) == coBitCast<coFloatx4>(_b); }
coFORCE_INLINE coQuat operator+(const coQuat& _q0, const coQuat& _q1)
{
	return coMake_quat(coBitCast<coFloatx4>(_q0) + coBitCast<coFloatx4>(_q1));
}
// coFORCE_INLINE coQuat operator*(const coQuat& _q, const coFloatx4& _xyzw)
// {
// 	coASSERT(false);
// 	return coMake_quat(coBitCast<coFloatx4>(_q) + _xyzw);
// }
coFORCE_INLINE coBool32x4 coIsValid(const coQuat& _this) { return coIsValid(coBitCast<coFloatx4>(_this)); }
void coSetupSquad(coQuat& _out0, coQuat& _out1, coQuat& _out3, const coQuat& _q0, const coQuat& _q1, const coQuat& _q2, const coQuat& _q3);
coFORCE_INLINE coFloatx3 coRotateVector(const coQuat& _this, const coFloatx3& _vec)
{
	// Alternative algo here: https://blog.molecular-matters.com/2013/05/24/a-faster-quaternion-vector-multiplication/
	// t = 2 * cross(q.xyz, v)
	// v' = v + q.w * t + cross(q.xyz, t)

	const coFloatx3 q = coBitCast<coFloatx3>(_this);
	const coFloatx3 w = coBroadcastW(q);
	const coFloatx3 r = 2.0f * coDot(q, _vec) * q + (w*w - coDot(q, q)) * _vec + 2.0f * w * coCross(q, _vec);
	return r;
}
coFORCE_INLINE coFloatx3 coInverseRotateVector(const coQuat& _this, const coFloatx3& _vec)
{
	return coRotateVector(coInverse(_this), _vec);
}
