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

coFORCE_INLINE void coSetRotation(coMat3& _this, const coQuat& _q)
{
	coASSERT(coIsNormalized(_q));

	{
		// Warning: Broken, check with identity.

		// Implementation found in Vectormath.
		// 	coFloatx4 xyzw_2, wwww, yzxw, zxyw, yzxw_2, zxyw_2;
		// 	coFloatx4 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;
		// 
		// 	const coFloatx4& q = coBitCast<coFloatx4>(_q);
		// 	xyzw_2 = q + q;
		// 	wwww = coBroadcastW(q);
		// 	yzxw = coShuffle<1, 2, 0, 3>(q, q);
		// 	zxyw = coShuffle<2, 0, 1, 3>(q, q);
		// 	yzxw_2 = coShuffle<1, 2, 0, 3>(xyzw_2, xyzw_2);
		// 	zxyw_2 = coShuffle<2, 0, 1, 3>(xyzw_2, xyzw_2);
		// 
		// 	tmp0 = yzxw_2 * wwww;
		// 	tmp1 = coFloatx4(1.0f) - yzxw * yzxw_2;
		// 	tmp2 = yzxw * xyzw_2;
		// 	tmp0 = zxyw * xyzw_2 + tmp0;
		// 	tmp1 = tmp1 - zxyw * zxyw_2;
		// 	tmp2 = tmp2 - zxyw_2 * wwww;
		// 
		// 	tmp3 = coSelectX(tmp0, tmp1);
		// 	tmp4 = coSelectX(tmp1, tmp2);
		// 	tmp5 = coSelectX(tmp2, tmp0);
		// 	_this.c0 = coBitCast<coVec3>(coSelectZ(tmp3, tmp2));
		// 	_this.c1 = coBitCast<coVec3>(coSelectZ(tmp4, tmp0));
		// 	_this.c1 = coBitCast<coVec3>(coSelectZ(tmp5, tmp1));
	}

	const coFloat qxx(_q.x * _q.x);
	const coFloat qyy(_q.y * _q.y);
	const coFloat qzz(_q.z * _q.z);
	const coFloat qxz(_q.x * _q.z);
	const coFloat qxy(_q.x * _q.y);
	const coFloat qyz(_q.y * _q.z);
	const coFloat qwx(_q.w * _q.x);
	const coFloat qwy(_q.w * _q.y);
	const coFloat qwz(_q.w * _q.z);

	_this.c0 = coVec3(1 - 2 * (qyy + qzz), 2 * (qxy + qwz), 2 * (qxz - qwy));
	_this.c1 = coVec3(2 * (qxy - qwz), 1 - 2 * (qxx + qzz), 2 * (qyz + qwx));
	_this.c2 = coVec3(2 * (qxz + qwy), 2 * (qyz - qwx), 1 - 2 * (qxx + qyy));
}
