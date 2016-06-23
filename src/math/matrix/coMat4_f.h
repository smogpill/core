// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/cppExtensions.h"
#include "math/matrix/coMat4.h"
#include "math/vector/coFloatx3.h"
#include "math/vector/coFloatx4_f.h"

coFORCE_INLINE coMat4 coMake(const coFloatx4& _c0, const coFloatx4& _c1, const coFloatx4& _c2, const coFloatx4& _c3) { return{ _c0, _c1, _c2, _c3 }; }
coMat4 coMakeLookAt(const coFloatx3& _eyePos, const coFloatx3& _lookAtPos, const coFloatx3& _upVec);
coMat4 coMakeOrthographic(float _left, float _right, float _bottom, float _top, float _zNear, float _zFar);
coMat4 coMakePerspective(float _fovyRadians, float _aspect, float _zNear, float _zFar);
void coSetUpperMat3(coMat4& _m4, const coMat3& _m3)
{
	_m4.c0 = coSelectXYZ(coMake_floatx4XYZ0(_m3.c0), _m4.c0);
	_m4.c1 = coSelectXYZ(coMake_floatx4XYZ0(_m3.c1), _m4.c1);
	_m4.c2 = coSelectXYZ(coMake_floatx4XYZ0(_m3.c2), _m4.c2);
}

coMat4 coTranspose(const coMat4& _a)
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

coBool32x4 coEqual(const coMat4& _m1, const coMat4& _m2, const coFloatx4& _epsilon = coMake_floatx4(0.0001f))
{
	const coBool32x4 b0 = coNearEqual(_m1.c0, _m2.c0, _epsilon);
	const coBool32x4 b1 = coNearEqual(_m1.c1, _m2.c1, _epsilon);
	const coBool32x4 b2 = coNearEqual(_m1.c2, _m2.c2, _epsilon);
	const coBool32x4 b3 = coNearEqual(_m1.c3, _m2.c3, _epsilon);
	return b0 && b1 && b2 && b3;
}