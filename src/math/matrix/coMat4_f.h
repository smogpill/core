// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"
#include "math/matrix/coMat3_f.h"
#include "math/matrix/coMat4.h"
#include "math/transform/coTransform.h"
#include "math/vector/coFloatx3.h"
#include "math/vector/coFloatx4_f.h"

coFORCE_INLINE coVec4 operator* (const coMat4& _this, const coVec4& _a)
{
	const coFloatx4 ax = coBroadcastX(_a);
	const coFloatx4 ay = coBroadcastY(_a);
	const coFloatx4 az = coBroadcastZ(_a);
	const coFloatx4 aw = coBroadcastW(_a);

	const coFloatx4 mx = _this.c0 * ax;
	const coFloatx4 my = _this.c1 * ay;
	const coFloatx4 mz = _this.c2 * az;
	const coFloatx4 mw = _this.c3 * aw;

	return coBitCast<coVec4>(mx + my + mz + mw);
}

coFORCE_INLINE coMat4 operator* (const coMat4& _a, const coMat4& _b)
{
	return coMat4(_a * _b.c0, _a * _b.c1, _a * _b.c2, _a * _b.c3);
}

coMat4 coMakeLookAt(const coVec3& _eyePos, const coVec3& _lookAtPos, const coVec3& _upVec);
coMat4 coMakeOrthographic(coFloat _left, coFloat _right, coFloat _bottom, coFloat _top, coFloat _zNear, coFloat _zFar);
void coSetPerspective(coMat4& _this, coFloat _fovyRadians, coFloat _aspect, coFloat _zNear, coFloat _zFar);
coFORCE_INLINE void coSetUpperMat3(coMat4& _m4, const coMat3& _m3)
{
	_m4.c0 = coSelectXYZ(coMake_floatx4XYZ0(_m3.c0), _m4.c0);
	_m4.c1 = coSelectXYZ(coMake_floatx4XYZ0(_m3.c1), _m4.c1);
	_m4.c2 = coSelectXYZ(coMake_floatx4XYZ0(_m3.c2), _m4.c2);
}

coFORCE_INLINE coMat4 coTranspose(const coMat4& _a)
{
	const coFloatx4 tmp0 = coShuffle<0, 1, 0, 1>(_a.c0, _a.c1);
	const coFloatx4 tmp2 = coShuffle<2, 3, 2, 3>(_a.c0, _a.c1);
	const coFloatx4 tmp1 = coShuffle<0, 1, 0, 1>(_a.c2, _a.c3);
	const coFloatx4 tmp3 = coShuffle<2, 3, 2, 3>(_a.c2, _a.c3);
	coMat4 out;
	out.c0 = coShuffle<0, 2, 0, 2>(tmp0, tmp1);
	out.c1 = coShuffle<1, 3, 1, 3>(tmp0, tmp1);
	out.c2 = coShuffle<0, 2, 0, 2>(tmp2, tmp3);
	out.c3 = coShuffle<1, 3, 1, 3>(tmp2, tmp3);
	return out;
}

coFORCE_INLINE coBool32x4 coEqual(const coMat4& _m1, const coMat4& _m2, const coFloatx4& _epsilon = coMake_floatx4(0.0001f))
{
	const coBool32x4 b0 = coNearEqual(_m1.c0, _m2.c0, _epsilon);
	const coBool32x4 b1 = coNearEqual(_m1.c1, _m2.c1, _epsilon);
	const coBool32x4 b2 = coNearEqual(_m1.c2, _m2.c2, _epsilon);
	const coBool32x4 b3 = coNearEqual(_m1.c3, _m2.c3, _epsilon);
	return b0 && b1 && b2 && b3;
}

coFORCE_INLINE void coSetWithoutScale(coMat4& _this, const coTransform& _t)
{
	coMat3 m;
	coSetRotation(m, _t.rotation);
	_this.c0 = coVec4(m.c0, 0.0f);
	_this.c1 = coVec4(m.c1, 0.0f);
	_this.c2 = coVec4(m.c2, 0.0f);
	_this.c3 = coVec4(_t.translation, 1.0f);
}

coFORCE_INLINE void coSetWithScale(coMat4& _this, const coTransform& _t)
{
	coSetWithoutScale(_this, _t);
	const coVec3 scale = _t.scale;
	_this.c0 *= coBroadcastX(coBitCast<coFloatx4>(scale));
	_this.c1 *= coBroadcastY(coBitCast<coFloatx4>(scale));
	_this.c2 *= coBroadcastZ(coBitCast<coFloatx4>(scale));
}
