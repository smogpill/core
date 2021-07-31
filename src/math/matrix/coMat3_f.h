// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"
#include "math/matrix/coMat3.h"
#include "math/vector/coVec3_f.h"
#include "math/quaternion/coQuat_f.h"

class coQuat;

coFORCE_INLINE coVec3 operator* (const coMat3& this_, const coVec3& a)
{
	const coFloatx3 ax = coBroadcastX(a);
	const coFloatx3 ay = coBroadcastY(a);
	const coFloatx3 az = coBroadcastZ(a);
	const coFloatx3 mx = this_.c0 * ax;
	const coFloatx3 my = this_.c1 * ay;
	const coFloatx3 mz = this_.c2 * az;
	return coBitCast<coVec3>(mx + my + mz);
}

coFORCE_INLINE coMat3 operator* (const coMat3& _a, const coMat3& _b)
{
	return coMat3(_a * _b.c0, _a * _b.c1, _a * _b.c2);
}

coFORCE_INLINE coMat3& operator+=(coMat3& this_, const coMat3& a)
{
	this_.c0 += a.c0;
	this_.c1 += a.c1;
	this_.c2 += a.c2;
	return this_;
}

coFORCE_INLINE coVec3 coGetScale(const coMat3& _a)
{
	return coVec3(coLength(_a.c0), coLength(_a.c1), coLength(_a.c2));
}

coFORCE_INLINE coMat3 coRemoveScale(const coMat3& _a)
{
	coMat3 m;
	m.c0 = coNormalize(_a.c0);
	m.c1 = coNormalize(_a.c1);
	m.c2 = coNormalize(_a.c2);
	return m;
}

coFORCE_INLINE coMat3 coTranspose(const coMat3& _a)
{
	const coFloatx3 tmp0 = coShuffle<0, 1, 0>(_a.c0, _a.c1);
	const coFloatx3 tmp1 = coShuffle<0, 1, 0>(_a.c2, _a.c2);
	const coFloatx3 tmp2 = coShuffle<2, 3, 2>(_a.c0, _a.c1);
	const coFloatx3 tmp3 = coShuffle<2, 3, 2>(_a.c2, _a.c2);
	coMat3 out;
	out.c0 = coShuffle<0, 2, 0>(tmp0, tmp1);
	out.c1 = coShuffle<1, 3, 1>(tmp0, tmp1);
	out.c2 = coShuffle<0, 2, 0>(tmp2, tmp3);
	return out;
}

coFORCE_INLINE void coMakeLookAt(coMat3& this_, const coVec3& from, const coVec3& to, const coVec3& up)
{
	const coVec3 z = coSafeNormalize(to - from);
	const coVec3 x = coSafeNormalize(coCross(up, z));
	const coVec3 y = coCross(z, x);
	this_ = coMat3(x, y, z);
}

coFORCE_INLINE void coMakeLookAt(coMat3& this_, const coVec3& from, const coVec3& to, coFloat roll)
{
	const coVec3 up = coVec3(coSin(roll), 0.0f, coCos(roll));
	coMakeLookAt(this_, from, to, up);
}

coFORCE_INLINE void coSetRotation(coMat3& _this, const coQuat& q)
{
	coASSERT(coIsNormalized(q));
	const coFloat x2 = q.x + q.x;  const coFloat y2 = q.y + q.y;  const coFloat z2 = q.z + q.z;
	const coFloat xx = q.x * x2;   const coFloat xy = q.x * y2;   const coFloat xz = q.x * z2;
	const coFloat yy = q.y * y2;   const coFloat yz = q.y * z2;   const coFloat zz = q.z * z2;
	const coFloat wx = q.w * x2;   const coFloat wy = q.w * y2;   const coFloat wz = q.w * z2;
	_this.c0 = coVec3(1.0f - (yy + zz), xy + wz, xz - wy);
	_this.c1 = coVec3(xy - wz, 1.0f - (xx + zz), yz + wx);
	_this.c2 = coVec3(xz + wy, yz - wx, 1.0f - (xx + yy));
}

coFORCE_INLINE coMat3 coInverse(const coMat3& m)
{
	// Impl from compute_inverse<3, 3, T, Q, Aligned> https://github.com/g-truc/glm/blob/master/glm/detail/func_matrix.inl (MIT license).
	const coFloat invDet = 1.0f / (
		+ m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
		- m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
		+ m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

	coMat3 inverse;
	inverse[0][0] = +(m[1][1] * m[2][2] - m[2][1] * m[1][2]) * invDet;
	inverse[1][0] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]) * invDet;
	inverse[2][0] = +(m[1][0] * m[2][1] - m[2][0] * m[1][1]) * invDet;
	inverse[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * invDet;
	inverse[1][1] = +(m[0][0] * m[2][2] - m[2][0] * m[0][2]) * invDet;
	inverse[2][1] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]) * invDet;
	inverse[0][2] = +(m[0][1] * m[1][2] - m[1][1] * m[0][2]) * invDet;
	inverse[1][2] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]) * invDet;
	inverse[2][2] = +(m[0][0] * m[1][1] - m[1][0] * m[0][1]) * invDet;

	return inverse;
}