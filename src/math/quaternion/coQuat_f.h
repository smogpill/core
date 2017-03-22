// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/scalar/coFloat_f.h"
#include "math/vector/coFloatx3_f.h"
#include "math/vector/coFloatx4_f.h"
#include "math/vector/coInt32x4_f.h"
#include "math/quaternion/coQuat.h"
#include "lang/coCppExtensions.h"

coFORCE_INLINE const coFloatx4& coMake_floatx4(const coQuat& _a) { return coBitCast<coFloatx4>(_a); }
coFORCE_INLINE const coQuat& coMake_quat(const coFloatx4& _a) { return coBitCast<coQuat>(_a); }
coFORCE_INLINE coQuat coMake_quat(float _x, float _y, float _z, float _w) { return coMake_quat(coMake_floatx4( _x, _y, _z, _w )); }
coFORCE_INLINE void coSetRotation(coQuat& _q, const coFloatx3& _fromUnitDir, const coFloatx3& _toUnitDir)
{
	// Implementation found at https://github.com/erwincoumans/sce_vectormath/blob/master/include/vectormath/SSE/cpp/quat_aos.h Quat::rotation(...)
	coASSERT(coIsNormalized(_fromUnitDir));
	coASSERT(coIsNormalized(_toUnitDir));
	const coFloatx3 cosAngle = coDot(_fromUnitDir, _toUnitDir);
	const coFloatx3 cosAngleX2Plus2 = cosAngle * coFloatx3(2.0f) + coFloatx3(2.0f);
	const coFloatx3 invCosHalfAngleX2 = coInvSqrtFast(cosAngleX2Plus2);
	const coFloatx3 cosHalfAngleX2 = invCosHalfAngleX2 * cosAngleX2Plus2;
	const coFloatx3 crossVec = coCross(_fromUnitDir, _toUnitDir);
	const coFloatx3 res = crossVec * invCosHalfAngleX2;
	coBitCast<coFloatx4>(_q) = coSelectXYZ(coBitCast<coFloatx4>(res), coBroadcastX(coBitCast<coFloatx4>(cosAngleX2Plus2) * coFloatx4(0.5f)));
}
coFORCE_INLINE coQuat coConjugate(const coQuat& _a) { return coBitCast<coQuat>(coBitCast<coInt32x4>(_a) ^ coBitCast<coInt32x4>(__m128_MASK_XYZ)); }
coFORCE_INLINE coQuat coExp(const coQuat& _a)
{
	const float r = coSqrt(_a.x*_a.x + _a.y*_a.y + _a.z*_a.z);
	const float expW = coExp(_a.w);
	const float s = (r >= 0.00001f) ? expW * coSin(r) / r : 0.0f;
	return coMake_quat(s*_a.x, s*_a.y, s*_a.z, expW * coCos(r));
}
coFORCE_INLINE coQuat coLn(const coQuat& _a)
{
	const float r = coSqrt(_a.x*_a.x + _a.y*_a.y + _a.z*_a.z);
	const float t = (r > 0.00001f) ? coAtan(r, _a.w) / r : 0.0f;
	return coMake_quat(t*_a.x, t*_a.y, t*_a.z, 0.5f * coLn(_a.x*_a.x + _a.y*_a.y + _a.z*_a.z + _a.w*_a.w));
}
coFORCE_INLINE coQuat coSyncWith(const coQuat& _a, const coQuat& _b)
{
	const coInt32x4 comp = coBitCast<coInt32x4>(coDot(coMake_floatx4(_a), coMake_floatx4(_b)) < coFloatx4_ZERO);
	return coBitCast<coQuat>(coBitCast<coInt32x4>(_a) ^ (comp & coBitCast<coInt32x4>(__m128_SIGN_MASK)));
}
coFORCE_INLINE coQuat operator*(const coQuat& _q0, const coQuat& _q1)
{
	// Wa * Wb - XYZa.XYZb + Wa*XYZb + Wb*XYZa + XYZa^XYZb
	// X = - Wa*Xb - Za*Yb - Ya*Zb + Xa*Wb
	// Y = - Za*Xb - Wa*Yb - Xa*Zb + Ya*Wb
	// Z = - Ya*Xb - Xa*Yb - Wa*Zb + Za*Wb
	// W = - Xa*Xb - Ya*Yb - Za*Zb + Wa*Wb
	const coFloatx4 a = coBitCast<coFloatx4>(_q0);
	const coFloatx4 b = coBitCast<coFloatx4>(_q1);
	const coFloatx4 Xb = coBroadcastX(b);
	const coFloatx4 Yb = coBroadcastY(b);
	const coFloatx4 Zb = coBroadcastZ(b);
	const coFloatx4 Wb = coBroadcastW(b);
	const coFloatx4 WZYXa = coShuffle<0, 1, 2, 3>(a, a);
	const coFloatx4 ZWXYa = coShuffle<1, 0, 3, 2>(a, a);
	const coFloatx4 YXWZa = coShuffle<2, 3, 0, 1>(a, a);
	const coFloatx4 XCol = WZYXa * Xb;
	const coFloatx4 YCol = ZWXYa * Yb;
	const coFloatx4 ZCol = YXWZa * Zb;
	const coFloatx4 WCol = a * Wb;
	return coMake_quat(WCol - (ZCol + XCol + YCol));
}
coFORCE_INLINE coQuat operator+(const coQuat& _q0, const coQuat& _q1)
{
	return coMake_quat(coBitCast<coFloatx4>(_q0) + coBitCast<coFloatx4>(_q1));
}
coFORCE_INLINE coQuat operator*(const coQuat& _q, const coFloatx4& _xyzw)
{
	coASSERT(false);
	return coMake_quat(coBitCast<coFloatx4>(_q) + _xyzw);
}
coFORCE_INLINE coBool32x4 coIsValid(const coQuat& _this) { return coIsValid(coBitCast<coFloatx4>(_this)); }
void coSetupSquad(coQuat& _out0, coQuat& _out1, coQuat& _out3, const coQuat& _q0, const coQuat& _q1, const coQuat& _q2, const coQuat& _q3);
coFORCE_INLINE coFloatx3 coTransformDirection(const coQuat& _this, const coFloatx3& _dir)
{
	const coFloatx3& q = coBitCast<coFloatx3>(_this);
	const coFloatx3 s = coBroadcastW(q);
	return 2.0f * coDot(q, _dir) * q + (s*s - coDot(q, q)) * _dir + 2.0f * s * coCross(q, _dir);
}
