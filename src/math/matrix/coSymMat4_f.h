#pragma once
#include "coSymMat4.h"
#include "../scalar/coFloat_f.h"

coFORCE_INLINE coFloat coFrobeniusNorm(const coSymMat4& this_)
{
	return coSquareRoot(
		(this_.m00 * this_.m00) + (this_.m01 * this_.m01) + (this_.m02 * this_.m02) + (this_.m03 * this_.m03) +
		(this_.m01 * this_.m01) + (this_.m11 * this_.m11) + (this_.m12 * this_.m12) + (this_.m13 * this_.m13) +
		(this_.m02 * this_.m02) + (this_.m12 * this_.m12) + (this_.m22 * this_.m22) + (this_.m23 * this_.m23) +
		(this_.m03 * this_.m03) + (this_.m13 * this_.m13) + (this_.m23 * this_.m23) + (this_.m33 * this_.m33));
}

coFORCE_INLINE coFloat coOff(const coSymMat4& this_)
{
	return coSquareRoot(2.0f * (
		(this_.m01 * this_.m01)	+ (this_.m02 * this_.m02) + (this_.m03 * this_.m03) +
		(this_.m12 * this_.m12) + (this_.m13 * this_.m13) +
		(this_.m23 * this_.m23)
		));
}

coFORCE_INLINE void coRot01(coSymMat4& this_, coFloat& c, coFloat& s)
{
	coCalcSymmetricGivensCoefficients(this_.m00, this_.m01, this_.m11, c, s);
	const coFloat cc = c * c;
	const coFloat ss = s * s;
	const coFloat mix = 2.0f * c * s * this_.m01;
// 	this_ = coSymMat3(cc * this_.m00 - mix + ss * this_.m11, 0.0f, c * this_.m02 - s * this_.m12,
// 		ss * this_.m00 + mix + cc * this_.m11, s * this_.m02 + c * this_.m12, this_.m22);
}

coFORCE_INLINE void coRot02(coSymMat4& this_, coFloat& c, coFloat& s)
{
	coCalcSymmetricGivensCoefficients(this_.m00, this_.m02, this_.m22, c, s);
	const coFloat cc = c * c;
	const coFloat ss = s * s;
	const coFloat mix = 2.0f * c * s * this_.m02;
// 	this_ = coSymMat3(cc * this_.m00 - mix + ss * this_.m22, c * this_.m01 - s * this_.m12, 0.0f,
// 		this_.m11, s * this_.m01 + c * this_.m12, ss * this_.m00 + mix + cc * this_.m22);
}

coFORCE_INLINE void coRot03(coSymMat4& this_, coFloat& c, coFloat& s)
{
	coCalcSymmetricGivensCoefficients(this_.m00, this_.m03, this_.m33, c, s);
	const coFloat cc = c * c;
	const coFloat ss = s * s;
	const coFloat mix = 2.0f * c * s * this_.m03;
}

coFORCE_INLINE void coRot12(coSymMat4& this_, coFloat& c, coFloat& s)
{
// 	coCalcSymmetricGivensCoefficients(this_.m11, this_.m12, this_.m22, c, s);
// 	const coFloat cc = c * c;
// 	const coFloat ss = s * s;
// 	const coFloat mix = 2.0f * c * s * this_.m12;
// 	this_ = coSymMat3(this_.m00, c * this_.m01 - s * this_.m02, s * this_.m01 + c * this_.m02,
// 		cc * this_.m11 - mix + ss * this_.m22, 0.0f, ss * this_.m11 + mix + cc * this_.m22);
}

coFORCE_INLINE void coRot13(coSymMat4& this_, coFloat& c, coFloat& s)
{
	coCalcSymmetricGivensCoefficients(this_.m11, this_.m13, this_.m33, c, s);
	const coFloat cc = c * c;
	const coFloat ss = s * s;
	const coFloat mix = 2.0f * c * s * this_.m13;
}

coFORCE_INLINE void coRot23(coSymMat4& this_, coFloat& c, coFloat& s)
{
	coCalcSymmetricGivensCoefficients(this_.m22, this_.m23, this_.m33, c, s);
	const coFloat cc = c * c;
	const coFloat ss = s * s;
	const coFloat mix = 2.0f * c * s * this_.m23;
}
