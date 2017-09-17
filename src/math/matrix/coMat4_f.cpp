// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "math/matrix/coMat4_f.h"

void coSetPerspective(coMat4& _this, coFloat _fovyRadians, coFloat _aspect, coFloat _zNear, coFloat _zFar)
{
	// Implementation found in Vectormath.

	coFloatx4 zero, col0, col1, col2, col3, tmp;
	const coFloat f = coTan(coFloat_halfPi - _fovyRadians * 0.5f);
	const coFloat rangeInv = 1.0f / (_zNear - _zFar);
	zero = coFloatx4(0.0f);
	tmp = zero;
	tmp.x = f / _aspect;
	_this.c0 = tmp;
	tmp = zero;
	tmp.y = f;
	_this.c1 = tmp;
	tmp = zero;
	tmp.z = (_zNear + _zFar) * rangeInv;
	tmp.w = -1.0f;
	_this.c2 = tmp;
	tmp = zero;
	tmp.z = _zNear * _zFar * rangeInv * 2.0f;
	_this.c3 = tmp;

// 	float f, rangeInv;
// 	__m128 zero, col0, col1, col2, col3;
// 	union { __m128 v; float s[4]; } tmp;
// 	f = tanf(_VECTORMATH_PI_OVER_2 - fovyRadians * 0.5f);
// 	rangeInv = 1.0f / (zNear - zFar);
// 	zero = _mm_setzero_ps();
// 	tmp.v = zero;
// 	tmp.s[0] = f / aspect;
// 	col0 = tmp.v;
// 	tmp.v = zero;
// 	tmp.s[1] = f;
// 	col1 = tmp.v;
// 	tmp.v = zero;
// 	tmp.s[2] = (zNear + zFar) * rangeInv;
// 	tmp.s[3] = -1.0f;
// 	col2 = tmp.v;
// 	tmp.v = zero;
// 	tmp.s[2] = zNear * zFar * rangeInv * 2.0f;
// 	col3 = tmp.v;
// 	return Matrix4(
// 		Vector4(col0),
// 		Vector4(col1),
// 		Vector4(col2),
// 		Vector4(col3)
// 	);


}
