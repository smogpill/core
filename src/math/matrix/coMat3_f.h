// Copyright(c) 2016-2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"
#include "math/matrix/coMat3.h"
#include "math/vector/coVec3_f.h"
#include "math/quaternion/coQuat_f.h"

class coQuat;

coMat3 coGetPolarDecompositionRotation(const coMat3& M, coFloat tolerance);

coFORCE_INLINE coVec3 operator* (const coMat3& this_, const coVec3& a)
{
	const coFloatx3 ax = coBroadcastX(a);
	const coFloatx3 ay = coBroadcastY(a);
	const coFloatx3 az = coBroadcastZ(a);
	const coFloatx3 mx = this_.c0 * ax;
	const coFloatx3 my = this_.c1 * ay;
	const coFloatx3 mz = this_.c2 * az;
	return coBitCast<coVec3>(mx + my + mz);
}

coFORCE_INLINE coMat3 operator* (const coMat3& _a, const coMat3& _b)
{
	return coMat3(_a * _b.c0, _a * _b.c1, _a * _b.c2);
}

coFORCE_INLINE coMat3& operator+=(coMat3& this_, const coMat3& a)
{
	this_.c0 += a.c0;
	this_.c1 += a.c1;
	this_.c2 += a.c2;
	return this_;
}

coFORCE_INLINE void coSetRow(coMat3& a, coUint rowIdx, const coVec3& value)
{
	a.c0[rowIdx] = value.x;
	a.c1[rowIdx] = value.y;
	a.c2[rowIdx] = value.z;
}

coFORCE_INLINE coVec3 coGetRow(const coMat3& a, const coUint rowIdx)
{
	return coVec3(a.c0[rowIdx], a.c1[rowIdx], a.c2[rowIdx]);
}

coFORCE_INLINE coVec3 coGetScale(const coMat3& _a)
{
	return coVec3(coLength(_a.c0), coLength(_a.c1), coLength(_a.c2));
}

coFORCE_INLINE coMat3 coRemoveScale(const coMat3& _a)
{
	coMat3 m;
	m.c0 = coNormalize(_a.c0);
	m.c1 = coNormalize(_a.c1);
	m.c2 = coNormalize(_a.c2);
	return m;
}

coFORCE_INLINE coMat3 coTranspose(const coMat3& a)
{
	// https://www.gamedev.net/forums/topic/692392-towards-an-optimal-vex-sse-33float-matrix-transpose/5357850/
	const __m128 T0 = _mm_unpacklo_ps(coBitCast<__m128>(a.c0), coBitCast<__m128>(a.c1));
	const __m128 T1 = _mm_unpackhi_ps(coBitCast<__m128>(a.c0), coBitCast<__m128>(a.c1));
	coMat3 out(nullptr);
	out.c0 = coBitCast<coVec3>(_mm_movelh_ps(T0, coBitCast<__m128>(a.c2)));
	out.c1 = coBitCast<coVec3>(_mm_shuffle_ps(T0, coBitCast<__m128>(a.c2), _MM_SHUFFLE(3, 1, 3, 2)));
	out.c2 = coBitCast<coVec3>(_mm_shuffle_ps(T1, coBitCast<__m128>(a.c2), _MM_SHUFFLE(3, 2, 1, 0)));
	return out;
}

coFORCE_INLINE void coMakeLookAt(coMat3& this_, const coVec3& from, const coVec3& to, const coVec3& up)
{
	const coVec3 z = coNormalize(to - from);
	const coVec3 x = coNormalize(coCross(up, z));
	const coVec3 y = coCross(z, x);
	this_ = coMat3(x, y, z);
}

coFORCE_INLINE void coMakeLookAt(coMat3& this_, const coVec3& from, const coVec3& to, coFloat roll)
{
	const coVec3 up = coVec3(coSin(roll), 0.0f, coCos(roll));
	coMakeLookAt(this_, from, to, up);
}

coFORCE_INLINE void coSetRotation(coMat3& _this, const coQuat& q)
{
	coASSERT(coIsNormalized(q));
	const coFloat x2 = q.x + q.x;  const coFloat y2 = q.y + q.y;  const coFloat z2 = q.z + q.z;
	const coFloat xx = q.x * x2;   const coFloat xy = q.x * y2;   const coFloat xz = q.x * z2;
	const coFloat yy = q.y * y2;   const coFloat yz = q.y * z2;   const coFloat zz = q.z * z2;
	const coFloat wx = q.w * x2;   const coFloat wy = q.w * y2;   const coFloat wz = q.w * z2;
	_this.c0 = coVec3(1.0f - (yy + zz), xy + wz, xz - wy);
	_this.c1 = coVec3(xy - wz, 1.0f - (xx + zz), yz + wx);
	_this.c2 = coVec3(xz + wy, yz - wx, 1.0f - (xx + yy));
}

coFORCE_INLINE void coSetRotation(coMat3& this_, const coFloatx3& eulerAngles)
{
	// todo
	coSetRotation(this_, coRotation(eulerAngles));
}

coFORCE_INLINE coMat3 coMakeOrthogonalFromZ(const coVec3& z, coVec3 x)
{
	const coVec3 y = coCross(z, x);
	x = coCross(y, z);
	return coMat3(coNormalize(x), coNormalize(y), coNormalize(z));
}

coFORCE_INLINE coMat3 coMakeOrthogonalFromXZ(const coVec3& x, coVec3 z)
{
	const coVec3 y = coCross(z, x);
	z = coCross(x, y);
	return coMat3(coNormalize(x), coNormalize(y), coNormalize(z));
}

coFORCE_INLINE coQuat coGetRotationNoScale(const coMat3& m)
{
	// Stolen from FQuat::FQuat(const FMatrix& M) from Unreal

	if (coAreAllTrue(coNearEqual0(m.c0)) || coAreAllTrue(coNearEqual0(m.c1)) || coAreAllTrue(coNearEqual0(m.c2)))
	{
		return coQuat::identity;
	}

	coQuat out;
	coFloat s;

	// Check diagonal (trace)
	const coFloat tr = m.c0.x + m.c1.y + m.c2.z;

	if (tr > 0.0f)
	{
		const coFloat invS = coInvSquareRoot(tr + 1.f);
		out.w = 0.5f * (1.f / invS);
		s = 0.5f * invS;

		out.x = (m.c1.z - m.c2.y) * s;
		out.y = (m.c2.x - m.c0.z) * s;
		out.z = (m.c0.y - m.c1.x) * s;
	}
	else
	{
		// diagonal is negative
		coInt32 i = 0;

		if (m.c1.y > m.c0.x)
			i = 1;

		if (m.c2.z > m[i][i])
			i = 2;

		static const coInt32 nxt[3] = { 1, 2, 0 };
		const coInt32 j = nxt[i];
		const coInt32 k = nxt[j];

		s = m[i][i] - m[j][j] - m[k][k] + 1.0f;

		coFloat InvS = coInvSquareRoot(s);

		coFloat qt[4];
		qt[i] = 0.5f * (1.f / InvS);

		s = 0.5f * InvS;

		qt[3] = (m[j][k] - m[k][j]) * s;
		qt[j] = (m[i][j] + m[j][i]) * s;
		qt[k] = (m[i][k] + m[k][i]) * s;

		out.x = qt[0];
		out.y = qt[1];
		out.z = qt[2];
		out.w = qt[3];
	}

	return out;
}

coFORCE_INLINE coMat3 coInverse(const coMat3& m)
{
	// Impl from compute_inverse<3, 3, T, Q, Aligned> https://github.com/g-truc/glm/blob/master/glm/detail/func_matrix.inl (MIT license).
	const coFloat invDet = 1.0f / (
		+ m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
		- m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
		+ m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

	coMat3 inverse;
	inverse[0][0] = +(m[1][1] * m[2][2] - m[2][1] * m[1][2]) * invDet;
	inverse[1][0] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]) * invDet;
	inverse[2][0] = +(m[1][0] * m[2][1] - m[2][0] * m[1][1]) * invDet;
	inverse[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * invDet;
	inverse[1][1] = +(m[0][0] * m[2][2] - m[2][0] * m[0][2]) * invDet;
	inverse[2][1] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]) * invDet;
	inverse[0][2] = +(m[0][1] * m[1][2] - m[1][1] * m[0][2]) * invDet;
	inverse[1][2] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]) * invDet;
	inverse[2][2] = +(m[0][0] * m[1][1] - m[1][0] * m[0][1]) * invDet;

	return inverse;
}

coFORCE_INLINE coFloatx3 coOneNorm(const coMat3& m)
{
	const coFloatx3 a = coSum(coAbs(m.c0));
	const coFloatx3 b = coSum(coAbs(m.c1));
	const coFloatx3 c = coSum(coAbs(m.c2));
	return coMax(a, coMax(b, c));
}

coFORCE_INLINE coFloatx3 coInfNorm(const coMat3& m)
{
	const coFloatx3 a = coAbs(coBroadcastX(m.c0)) + coAbs(coBroadcastX(m.c1)) + coAbs(coBroadcastX(m.c2));
	const coFloatx3 b = coAbs(coBroadcastY(m.c0)) + coAbs(coBroadcastY(m.c1)) + coAbs(coBroadcastY(m.c2));
	const coFloatx3 c = coAbs(coBroadcastZ(m.c0)) + coAbs(coBroadcastZ(m.c1)) + coAbs(coBroadcastZ(m.c2));
	return coMax(a, coMax(b, c));
}