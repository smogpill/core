// Copyright(c) 2016-2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"
#include "math/matrix/coMat3_f.h"
#include "math/matrix/coMat4.h"
#include "math/transform/coTransform.h"
#include "math/vector/coFloatx3.h"
#include "math/vector/coFloatx4_f.h"

coFORCE_INLINE coVec4 operator* (const coVec4& _a, const coMat4& _this)
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

coFORCE_INLINE coMat4 operator* (const coMat4& a, const coMat4& b)
{
	coMat4 o;
	coVec4 tmp;

	tmp = coBroadcastX(a[0]) * b[0];
	tmp = coMulAdd(coBroadcastY(a[0]), b[1], tmp);
	tmp = coMulAdd(coBroadcastZ(a[0]), b[2], tmp);
	o.c0 = coMulAdd(coBroadcastW(a[0]), b[3], tmp);

	tmp = coBroadcastX(a[1]) * b[0];
	tmp = coMulAdd(coBroadcastY(a[1]), b[1], tmp);
	tmp = coMulAdd(coBroadcastZ(a[1]), b[2], tmp);
	o.c1 = coMulAdd(coBroadcastW(a[1]), b[3], tmp);

	tmp = coBroadcastX(a[2]) * b[0];
	tmp = coMulAdd(coBroadcastY(a[2]), b[1], tmp);
	tmp = coMulAdd(coBroadcastZ(a[2]), b[2], tmp);
	o.c2 = coMulAdd(coBroadcastW(a[2]), b[3], tmp);

	tmp = coBroadcastX(a[3]) * b[0];
	tmp = coMulAdd(coBroadcastY(a[3]), b[1], tmp);
	tmp = coMulAdd(coBroadcastZ(a[3]), b[2], tmp);
	o.c3 = coMulAdd(coBroadcastW(a[3]), b[3], tmp);

	return o;
}

coFORCE_INLINE coMat4 operator* (const coMat4& m, const coFloat& f)
{
	return coMat4(m.c0 * f, m.c1 * f, m.c2 * f, m.c3 * f);
}

coFORCE_INLINE coMat4& operator+=(coMat4& this_, const coMat4& a)
{
	this_.c0 += a.c0;
	this_.c1 += a.c1;
	this_.c2 += a.c2;
	this_.c3 += a.c3;
	return this_;
}

coFORCE_INLINE coVec3 coTransformVector(const coMat4& m, const coVec3& v)
{
	return coVec3(coVec4(v, 0.0f) * m);
}

coFORCE_INLINE coVec3 coTransformPosition(const coMat4& m, const coVec3& v)
{
	return coVec3(coVec4(v, 1.0f) * m);
}

void coMakeLookAt(coMat4& this_, const coVec3& eyePos, const coVec3& targetPos, const coVec3& up);
void coMakeLookAt(coMat4& this_, const coVec3& eyePos, const coVec3& targetPos, coFloat roll);
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
	const coFloatx4 tmp1 = coShuffle<0, 1, 0, 1>(_a.c2, _a.c3);
	const coFloatx4 tmp2 = coShuffle<2, 3, 2, 3>(_a.c0, _a.c1);
	const coFloatx4 tmp3 = coShuffle<2, 3, 2, 3>(_a.c2, _a.c3);
	coMat4 out;
	out.c0 = coShuffle<0, 2, 0, 2>(tmp0, tmp1);
	out.c1 = coShuffle<1, 3, 1, 3>(tmp0, tmp1);
	out.c2 = coShuffle<0, 2, 0, 2>(tmp2, tmp3);
	out.c3 = coShuffle<1, 3, 1, 3>(tmp2, tmp3);
	return out;
}

coFORCE_INLINE coQuat coGetRotationNoScale(const coMat4& m)
{
	return coGetRotationNoScale(static_cast<const coMat3&>(m));
}

coFORCE_INLINE coMat4 coRemoveScale(const coMat4& m)
{
	coMat4 o(nullptr);
	o.c0 = coNormalize(m.c0);
	o.c1 = coNormalize(m.c1);
	o.c2 = coNormalize(m.c2);
	o.c3 = m.c3;
	return o;
}

coFORCE_INLINE coVec3 coGetScaleXYZ(const coMat4& m)
{
	return coVec3(coLength(m.c0), coLength(m.c1), coLength(m.c2));
}

coFORCE_INLINE coMat4 coInverse(const coMat4& m)
{
	// Impl from: compute_inverse<4, 4, T, Q, Aligned> in https://github.com/g-truc/glm/blob/master/glm/detail/func_matrix.inl (MIT license)
	const coFloat coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	const coFloat coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
	const coFloat coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

	const coFloat coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	const coFloat coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
	const coFloat coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

	const coFloat coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	const coFloat coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
	const coFloat coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

	const coFloat coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	const coFloat coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
	const coFloat coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

	const coFloat coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	const coFloat coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
	const coFloat coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

	const coFloat coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	const coFloat coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
	const coFloat coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

	const coVec4 fac0(coef00, coef00, coef02, coef03);
	const coVec4 fac1(coef04, coef04, coef06, coef07);
	const coVec4 fac2(coef08, coef08, coef10, coef11);
	const coVec4 fac3(coef12, coef12, coef14, coef15);
	const coVec4 fac4(coef16, coef16, coef18, coef19);
	const coVec4 fac5(coef20, coef20, coef22, coef23);

	const coVec4 vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
	const coVec4 vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
	const coVec4 vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
	const coVec4 vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

	const coVec4 inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
	const coVec4 inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
	const coVec4 inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
	const coVec4 inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);

	const coVec4 signA(+1, -1, +1, -1);
	const coVec4 signB(-1, +1, -1, +1);
	const coMat4 inverse(inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB);

	const coVec4 row0(inverse[0][0], inverse[1][0], inverse[2][0], inverse[3][0]);

	const coVec4 dot0(m[0] * row0);
	const coFloat dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

	const coFloat invDet = 1.0f / dot1;

	return inverse * invDet;
}

/*
coFORCE_INLINE coMat4 coInverseAffineNoScale(const coMat4& _this)
{
	// https://lxjk.github.io/2017/09/03/Fast-4x4-Matrix-Inverse-with-SSE-SIMD-Explained.html
	coMat4 m;

	// transpose 3x3, we know m03 = m13 = m23 = 0
	__m128 t0 = VecShuffle_0101(inM.mVec[0], inM.mVec[1]); // 00, 01, 10, 11
	__m128 t1 = VecShuffle_2323(inM.mVec[0], inM.mVec[1]); // 02, 03, 12, 13
	m.c0 = VecShuffle(t0, inM.mVec[2], 0, 2, 0, 3); // 00, 10, 20, 23(=0)
	m.c1 = VecShuffle(t0, inM.mVec[2], 1, 3, 1, 3); // 01, 11, 21, 23(=0)
	m.c2 = VecShuffle(t1, inM.mVec[2], 0, 2, 2, 3); // 02, 12, 22, 23(=0)

	// last line
	m.c3] = _mm_mul_ps(r.mVec[0], VecSwizzle1(inM.mVec[3], 0));
	m.c3] = _mm_add_ps(r.mVec[3], _mm_mul_ps(r.mVec[1], VecSwizzle1(inM.mVec[3], 1)));
	m.c3] = _mm_add_ps(r.mVec[3], _mm_mul_ps(r.mVec[2], VecSwizzle1(inM.mVec[3], 2)));
	m.c3] = _mm_sub_ps(_mm_setr_ps(0.f, 0.f, 0.f, 1.f), r.mVec[3]);

	return r;
}
*/

/*
coFORCE_INLINE coMat4 coInverseAffine(const coMat4& _this)
{
	Matrix4 r;

	// transpose 3x3, we know m03 = m13 = m23 = 0
	__m128 t0 = VecShuffle_0101(inM.mVec[0], inM.mVec[1]); // 00, 01, 10, 11
	__m128 t1 = VecShuffle_2323(inM.mVec[0], inM.mVec[1]); // 02, 03, 12, 13
	r.mVec[0] = VecShuffle(t0, inM.mVec[2], 0, 2, 0, 3); // 00, 10, 20, 23(=0)
	r.mVec[1] = VecShuffle(t0, inM.mVec[2], 1, 3, 1, 3); // 01, 11, 21, 23(=0)
	r.mVec[2] = VecShuffle(t1, inM.mVec[2], 0, 2, 2, 3); // 02, 12, 22, 23(=0)

	// (SizeSqr(mVec[0]), SizeSqr(mVec[1]), SizeSqr(mVec[2]), 0)
	__m128 sizeSqr;
	sizeSqr = _mm_mul_ps(r.mVec[0], r.mVec[0]);
	sizeSqr = _mm_add_ps(sizeSqr, _mm_mul_ps(r.mVec[1], r.mVec[1]));
	sizeSqr = _mm_add_ps(sizeSqr, _mm_mul_ps(r.mVec[2], r.mVec[2]));

	// optional test to avoid divide by 0
	__m128 one = _mm_set1_ps(1.f);
	// for each component, if(sizeSqr < SMALL_NUMBER) sizeSqr = 1;
	__m128 rSizeSqr = _mm_blendv_ps(
		_mm_div_ps(one, sizeSqr),
		one,
		_mm_cmplt_ps(sizeSqr, _mm_set1_ps(SMALL_NUMBER))
	);

	r.mVec[0] = _mm_mul_ps(r.mVec[0], rSizeSqr);
	r.mVec[1] = _mm_mul_ps(r.mVec[1], rSizeSqr);
	r.mVec[2] = _mm_mul_ps(r.mVec[2], rSizeSqr);

	// last line
	r.mVec[3] = _mm_mul_ps(r.mVec[0], VecSwizzle1(inM.mVec[3], 0));
	r.mVec[3] = _mm_add_ps(r.mVec[3], _mm_mul_ps(r.mVec[1], VecSwizzle1(inM.mVec[3], 1)));
	r.mVec[3] = _mm_add_ps(r.mVec[3], _mm_mul_ps(r.mVec[2], VecSwizzle1(inM.mVec[3], 2)));
	r.mVec[3] = _mm_sub_ps(_mm_setr_ps(0.f, 0.f, 0.f, 1.f), r.mVec[3]);

	return r;
}
*/

coFORCE_INLINE coBool32x4 coNearEqual(const coMat4& _m1, const coMat4& _m2, const coFloatx4& _epsilon = coMake_floatx4(0.0001f))
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

coFORCE_INLINE coMat4::coMat4(const coTransform& t)
{
	coSetWithScale(*this, t);
}

coFORCE_INLINE void coSetTranslation(coMat4& _this, const coVec3& v)
{
	_this.c3 = coVec4(v, 1.0f);
}
coFORCE_INLINE void coSetRotationNoScale(coMat4& _this, const coQuat& q)
{
	coMat3 m;
	coSetRotation(m, q);
	coSetUpperMat3(_this, m);
}

//! X: Roll, Y: Pitch, Z: Yaw
coFORCE_INLINE void coSetRotationNoScale(coMat4& this_, const coFloatx3& eulerAngles)
{
	coMat3 m;
	coSetRotation(m, eulerAngles);
	coSetUpperMat3(this_, m);
}

coFORCE_INLINE coMat4 coRotate(const coMat4& m, const coQuat& v)
{
	coMat4 r;
	coSetRotationNoScale(r, v);
	return r * m;
}

coFORCE_INLINE coMat4 coTranslate(const coMat4& m, const coVec3& v)
{
	coMat4 t;
	coSetTranslation(t, v);
	return t * m;
}

coFORCE_INLINE coMat4 coScale(const coMat4& m, const coVec3& v)
{
	coMat4 o = m;
	o.c0 *= v.x;
	o.c1 *= v.y;
	o.c2 *= v.z;
	return o;
}
