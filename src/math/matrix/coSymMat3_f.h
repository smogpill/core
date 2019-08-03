// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coSymMat3.h"
#include "../scalar/coFloat_f.h"

coFORCE_INLINE coFloat coFNorm(const coSymMat3& this_)
{
	return coSquareRoot(
		(this_.m00 * this_.m00) 
		+ (this_.m01 * this_.m01) + (this_.m02 * this_.m02)
		+ (this_.m01 * this_.m01) + (this_.m11 * this_.m11) + (this_.m12 * this_.m12)
		+ (this_.m02 * this_.m02) + (this_.m12 * this_.m12) + (this_.m22 * this_.m22));
}

coFORCE_INLINE coFloat coOff(const coSymMat3& this_)
{
	return coSquareRoot(2.0f * ((this_.m01 * this_.m01) + (this_.m02 * this_.m02) + (this_.m12 * this_.m12)));
}

coFORCE_INLINE coSymMat3 coMul_ata(const coMat3& a)
{
	return coSymMat3(
		a[0][0] * a[0][0] + a[1][0] * a[1][0] + a[2][0] * a[2][0],
		a[0][0] * a[0][1] + a[1][0] * a[1][1] + a[2][0] * a[2][1],
		a[0][0] * a[0][2] + a[1][0] * a[1][2] + a[2][0] * a[2][2],
		a[0][1] * a[0][1] + a[1][1] * a[1][1] + a[2][1] * a[2][1],
		a[0][1] * a[0][2] + a[1][1] * a[1][2] + a[2][1] * a[2][2],
		a[0][2] * a[0][2] + a[1][2] * a[1][2] + a[2][2] * a[2][2]);
}

coFORCE_INLINE coVec3 coVmul(const coSymMat3& this_, const coVec3& v)
{
	coVec3 o;
	o.x = (this_.m00 * v.x) + (this_.m01 * v.y) + (this_.m02 * v.z);
	o.y = (this_.m01 * v.x) + (this_.m11 * v.y) + (this_.m12 * v.z);
	o.z = (this_.m02 * v.x) + (this_.m12 * v.y) + (this_.m22 * v.z);
	return o;
}

coFORCE_INLINE void coCalcSymmetricGivensCoefficients(coFloat a_pp, coFloat a_pq, coFloat a_qq, coFloat& c, coFloat& s)
{
	if (a_pq == 0)
	{
		c = 1.0f;
		s = 0.0f;
		return;
	}

	const coFloat tau = (a_qq - a_pp) / (2.0f * a_pq);
	const coFloat stt = coSquareRoot(1.0f + tau * tau);
	const coFloat tan = 1.0f / ((tau >= 0.0f) ? (tau + stt) : (tau - stt));
	c = 1.0f / coSquareRoot(1.0f + tan * tan);
	s = tan * c;
}

coFORCE_INLINE void coRot01(coSymMat3& this_, coFloat& c, coFloat& s)
{
	coCalcSymmetricGivensCoefficients(this_.m00, this_.m01, this_.m11, c, s);
	const coFloat cc = c * c;
	const coFloat ss = s * s;
	const coFloat mix = 2.0f * c * s * this_.m01;
	this_ = coSymMat3(cc * this_.m00 - mix + ss * this_.m11, 0.0f, c * this_.m02 - s * this_.m12,
		ss * this_.m00 + mix + cc * this_.m11, s * this_.m02 + c * this_.m12, this_.m22);
}

coFORCE_INLINE void coRot02(coSymMat3& this_, coFloat& c, coFloat& s)
{
	coCalcSymmetricGivensCoefficients(this_.m00, this_.m02, this_.m22, c, s);
	const coFloat cc = c * c;
	const coFloat ss = s * s;
	const coFloat mix = 2.0f * c * s * this_.m02;
	this_ = coSymMat3(cc * this_.m00 - mix + ss * this_.m22, c * this_.m01 - s * this_.m12, 0.0f,
		this_.m11, s * this_.m01 + c * this_.m12, ss * this_.m00 + mix + cc * this_.m22);
}

coFORCE_INLINE void coRot12(coSymMat3& this_, coFloat& c, coFloat& s)
{
	coCalcSymmetricGivensCoefficients(this_.m11, this_.m12, this_.m22, c, s);
	const coFloat cc = c * c;
	const coFloat ss = s * s;
	const coFloat mix = 2.0f * c * s * this_.m12;
	this_ = coSymMat3(this_.m00, c * this_.m01 - s * this_.m02, s * this_.m01 + c * this_.m02,
		cc * this_.m11 - mix + ss * this_.m22, 0.0f, ss * this_.m11 + mix + cc * this_.m22);
}
