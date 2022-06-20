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
	// More here: https://github.com/g-truc/glm/blob/master/glm/ext/matrix_clip_space.inl

	coASSERT(_aspect > 1e-4f);
	coASSERT(_zFar - _zNear > 0.0001f);
	const coFloat tanHalfFOV = coTan(_fovyRadians * 0.5f);
	_this.c0 = coVec4(1.0f / (_aspect * tanHalfFOV), 0.0f, 0.0f, 0.0f);
	_this.c1 = coVec4(0.0f, 1.0f / tanHalfFOV, 0.0f, 0.0f);
	_this.c2 = coVec4(0.0f, 0.0f, ((_zNear == _zFar) ? 1.0f : _zFar / (_zFar - _zNear)), 1.0f);
	_this.c3 = coVec4(0.0f, 0.0f, -_zNear * ((_zNear == _zFar) ? 1.0f : _zFar / (_zFar - _zNear)), 0.0f);

	/*
	const coFloat tanHalfFOV = coTan(_fovyRadians * 0.5f);
	_this.c0 = coVec4(1.0f / (_aspect * tanHalfFOV), 0.0f, 0.0f, 0.0f);
	_this.c1 = coVec4(0.0f, 1.0f / tanHalfFOV, 0.0f, 0.0f);
	_this.c2 = coVec4(0.0f, 0.0f, ((_zNear == _zFar) ? 1.0f : (_zFar + _zNear) / (_zFar - _zNear)), 1.0f);
	_this.c3 = coVec4(0.0f, 0.0f, -_zNear * ((_zNear == _zFar) ? 1.0f : 2.0f * _zFar / (_zFar - _zNear)), 0.0f);
	*/
}
