// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "math/matrix/coMat4_f.h"

void coSetPerspective(coMat4& _this, coFloat _fovyRadians, coFloat _aspect, coFloat _zNear, coFloat _zFar)
{
// 	const float h = 1.0f / tan(fov*PI_OVER_360);
// 	float neg_depth = zNear - zFar;
// 
// 	m[0] = h / aspect;
// 	m[1] = 0;
// 	m[2] = 0;
// 	m[3] = 0;
// 
// 	m[4] = 0;
// 	m[5] = h;
// 	m[6] = 0;
// 	m[7] = 0;
// 
// 	m[8] = 0;
// 	m[9] = 0;
// 	m[10] = (zFar + zNear) / neg_depth;
// 	m[11] = -1;
// 
// 	m[12] = 0;
// 	m[13] = 0;
// 	m[14] = 2.0f*(zNear*zFar) / neg_depth;
// 	m[15] = 0;

	const coFloat h = 1.0f / coTan(_fovyRadians * 0.5f);
	const coFloat rangeInv = 1.0f / (_zNear - _zFar);
	_this.c0 = coFloatx4(h / _aspect, 0, 0, 0);
	_this.c1 = coFloatx4(0, h, 0, 0);
	_this.c2 = coFloatx4(0, 0, (_zNear + _zFar) * rangeInv, -1);
	_this.c3 = coFloatx4(0, 0, 2.0f * _zNear * _zFar * rangeInv, 0);

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
