// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"
#include "math/matrix/coMat3.h"
#include "math/vector/coVec3_f.h"
#include "math/quaternion/coQuat_f.h"

class coQuat;

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

coFORCE_INLINE void coSetRotation(coMat3& _this, const coQuat& _q)
{
	// Implementation found in Vectormath.

	coASSERT(coIsNormalized(_q));
	coFloatx4 xyzw_2, wwww, yzxw, zxyw, yzxw_2, zxyw_2;
	coFloatx4 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;

	const coFloatx4& q = coBitCast<coFloatx4>(_q);
	xyzw_2 = q + q;
	wwww = coBroadcastW(q);
	yzxw = coShuffle<1, 2, 0, 3>(q, q);
	zxyw = coShuffle<2, 0, 1, 3>(q, q);
	yzxw_2 = coShuffle<1, 2, 0, 3>(xyzw_2, xyzw_2);
	zxyw_2 = coShuffle<2, 0, 1, 3>(xyzw_2, xyzw_2);

	tmp0 = yzxw_2 * wwww;
	tmp1 = coFloatx4(1.0f) - yzxw * yzxw_2;
	tmp2 = yzxw * xyzw_2;
	tmp0 = zxyw * xyzw_2 + tmp0;
	tmp1 = tmp1 - zxyw * zxyw_2;
	tmp2 = tmp2 - zxyw_2 * wwww;

	tmp3 = coSelectX(tmp0, tmp1);
	tmp4 = coSelectX(tmp1, tmp2);
	tmp5 = coSelectX(tmp2, tmp0);
	_this.c0 = coBitCast<coVec3>(coSelectZ(tmp3, tmp2));
	_this.c1 = coBitCast<coVec3>(coSelectZ(tmp4, tmp0));
	_this.c1 = coBitCast<coVec3>(coSelectZ(tmp5, tmp1));
}
