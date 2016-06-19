// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/cppExtensions.h"
#include "math/matrix/mat4.h"
#include "math/vector/floatx3.h"
#include "math/vector/floatx4_f.h"

namespace co
{
	coFORCE_INLINE Mat4 make(const Floatx4& _c0, const Floatx4& _c1, const Floatx4& _c2, const Floatx4& _c3) { return { _c0, _c1, _c2, _c3 }; }
	Mat4 makeLookAt(const Floatx3& _eyePos, const Floatx3& _lookAtPos, const Floatx3& _upVec);
	Mat4 makeOrthographic(float _left, float _right, float _bottom, float _top, float _zNear, float _zFar);
	Mat4 makePerspective(float _fovyRadians, float _aspect, float _zNear, float _zFar);
	void setUpperMat3(Mat4& _m4, const Mat3& _m3)
	{
		_m4.c0 = selectXYZ(make_floatx4XYZ0(_m3.c0), _m4.c0);
		_m4.c1 = selectXYZ(make_floatx4XYZ0(_m3.c1), _m4.c1);
		_m4.c2 = selectXYZ(make_floatx4XYZ0(_m3.c2), _m4.c2);
	}

	Mat4 transpose(const Mat4& _a)
	{
		const Floatx4 tmp0 = shuffle<0, 1, 0, 1>(_a.c0, _a.c1);
		const Floatx4 tmp2 = shuffle<2, 3, 2, 3>(_a.c0, _a.c1);
		const Floatx4 tmp1 = shuffle<0, 1, 0, 1>(_a.c2, _a.c3);
		const Floatx4 tmp3 = shuffle<2, 3, 2, 3>(_a.c2, _a.c3);
		Mat4 out;
		out.c0 = shuffle<0, 2, 0, 2>(tmp0, tmp1);
		out.c1 = shuffle<1, 3, 1, 3>(tmp0, tmp1);
		out.c2 = shuffle<0, 2, 0, 2>(tmp2, tmp3);
		out.c3 = shuffle<1, 3, 1, 3>(tmp2, tmp3);
		return out;
	}

	Bool32x4 equal(const Mat4& _m1, const Mat4& _m2, const Floatx4& _epsilon = make_floatx4(0.0001f))
	{
		const Bool32x4 b0 = nearEqual(_m1.c0, _m2.c0, _epsilon);
		const Bool32x4 b1 = nearEqual(_m1.c1, _m2.c1, _epsilon);
		const Bool32x4 b2 = nearEqual(_m1.c2, _m2.c2, _epsilon);
		const Bool32x4 b3 = nearEqual(_m1.c3, _m2.c3, _epsilon);
		return b0 && b1 && b2 && b3;
	}
}
