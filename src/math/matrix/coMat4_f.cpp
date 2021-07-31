// Copyright(c) 2016-2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "math/matrix/coMat4_f.h"

void coMakeLookAt(coMat4& this_, const coVec3& eyePos, const coVec3& targetPos, const coVec3& up)
{
	coMat3 m3;
	coMakeLookAt(m3, eyePos, targetPos, up);
	coSetUpperMat3(this_, m3);
	this_.c0.w = -coDot(m3.c0, eyePos).x;
	this_.c1.w = -coDot(m3.c1, eyePos).x;
	this_.c2.w = -coDot(m3.c2, eyePos).x;
	this_.c3.w = 1.0f;
}

void coMakeLookAt(coMat4& this_, const coVec3& eyePos, const coVec3& targetPos, coFloat roll)
{
	coMat3 m3;
	coMakeLookAt(m3, eyePos, targetPos, roll);
	coSetUpperMat3(this_, m3);
	this_.c0.w = -coDot(m3.c0, eyePos).x;
	this_.c1.w = -coDot(m3.c1, eyePos).x;
	this_.c2.w = -coDot(m3.c2, eyePos).x;
	this_.c3.w = 1.0f;
}

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

	coASSERT(_aspect > 0.0001f);
	coASSERT(_zFar - _zNear > 0.0001f);

	const coFloat h = 1.0f / coTan(_fovyRadians * 0.5f);
	const coFloat rangeInv = 1.0f / (_zFar -_zNear);
	_this.c0 = coFloatx4(h / _aspect, 0, 0, 0);
	_this.c1 = coFloatx4(0, h, 0, 0);
	_this.c2 = coFloatx4(0, 0, _zFar * rangeInv, 1);
	_this.c3 = coFloatx4(0, 0, -_zFar * _zNear * rangeInv, 0);

	_this = coTranspose(_this);

// 	assert(abs(aspect - std::numeric_limits<T>::epsilon()) > static_cast<T>(0));
// 
// 	T const tanHalfFovy = tan(fovy / static_cast<T>(2));
// 
// 	mat<4, 4, T, defaultp> Result(static_cast<T>(0));
// 	Result[0][0] = static_cast<T>(1) / (aspect * tanHalfFovy);
// 	Result[1][1] = static_cast<T>(1) / (tanHalfFovy);
// 	Result[2][3] = static_cast<T>(1);
// 
// #		if GLM_DEPTH_CLIP_SPACE == GLM_DEPTH_ZERO_TO_ONE
// 	Result[2][2] = zFar / (zFar - zNear);
// 	Result[3][2] = -(zFar * zNear) / (zFar - zNear);
// #		else
// 	Result[2][2] = (zFar + zNear) / (zFar - zNear);
// 	Result[3][2] = -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
// #		endif
// 
// 	return Result;

}
