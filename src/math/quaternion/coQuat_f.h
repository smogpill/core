// Copyright(c) 2016-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/scalar/coFloat_f.h"
#include "math/vector/coVec3_f.h"
#include "math/vector/coVec4_f.h"
#include "math/vector/coInt32x4_f.h"
#include "math/quaternion/coQuat.h"
#include "lang/coCppExtensions.h"
#include "debug/log/coLog.h"

coFORCE_INLINE coQuat coNormalize(const coQuat& _this)
{
	return coQuat(coNormalize(static_cast<const coVec4&>(_this)));
}
coFORCE_INLINE coBool32x4 coIsNormalized(const coQuat& _this, const coVec4& _squareEpsilon = coVec4(1e-3f))
{
	return coIsNormalized(coBitCast<coVec4>(_this), _squareEpsilon);
}

//! X: Roll, Y: Pitch, Z: Yaw
coFORCE_INLINE coQuat coRotation(const coFloatx3& _eulerAngles)
{
	const coFloatx3 halfAngles = _eulerAngles * 0.5f;
	const coFloatx3 s = coSin(halfAngles);
	const coFloatx3 c = coCos(halfAngles);

	const coFloat sy = s.z;
	const coFloat cy = c.z;
	const coFloat sp = s.y;
	const coFloat cp = c.y;
	const coFloat sr = s.x;
	const coFloat cr = c.x;

	coQuat r(nullptr);

	// UE4 order (FRotator::Quaternion())
	r.x = +cr*sp*sy - sr*cp*cy;
	r.y = -cr*sp*cy - sr*cp*sy;
	r.z = +cr*cp*sy - sr*sp*cy;
	r.w = +cr*cp*cy + sr*sp*sy;

	/*
	Wikipedia order https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
	r.x = sr * cp * cy - cr * sp * sy;
	r.y = cr * sp * cy + sr * cp * sy;
	r.z = cr * cp * sy - sr * sp * cy;
	r.w = cr * cp * cy + sr * sp * sy;
	*/

	r  = coNormalize(r); // HACK
	return r;
}

coFORCE_INLINE coFloatx3 coGetEulerAngles(const coQuat& q)
{
	coFloatx3 out;

	// Roll
	const coFloat sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	const coFloat cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	out.x = coAtan(cosr_cosp, sinr_cosp);

	// Pitch
	const coFloat sinp = 2 * (q.w * q.y - q.z * q.x);
	if (coAbs(sinp) >= 1.0f)
		out.y = coCopySign(coFloat_halfPi, sinp); // Use 90 degrees if out of range
	else
		out.y = coAsin(sinp);

	// Yaw
	const coFloat siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	const coFloat cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	out.z = coAtan(cosy_cosp, siny_cosp);

	return out;
}

coFORCE_INLINE coQuat coRotation(const coVec3& _axis, coFloat _angle)
{
	coASSERT(coIsNormalized(_axis));
	const coFloatx4 halfAngle = _angle * 0.5f;
	const coFloatx4 s = coSin(halfAngle);
	const coFloatx4 c = coCos(halfAngle);
	const coQuat q = coBitCast<coQuat>(coSelectXYZ(coBitCast<coFloatx4>(_axis) * s, c));
	return q;
}
coFORCE_INLINE coQuat _coRotation(const coVec3& _from, const coVec3& _to, coFloatx3 _fromToNorm)
{
	// http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/index.htm
	// http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/minorlogic.htm
	const coFloatx3 w = _fromToNorm + coDot(_from, _to);
	if (w > 1e-6f * _fromToNorm)
	{
		const coFloatx3 axis = coCross(_from, _to);
		const coFloatx4 q = coSelectXYZ(coBitCast<coFloatx4>(axis), coBroadcastX(coBitCast<coFloatx4>(w)));
		return coNormalize(coBitCast<coQuat>(q));
	}
	else
	{
		//return coNormalize(coAbs(_from.x) > coAbs(_from.y) ? coQuat(-_from.z, 0.0f, _from.x, 0.0f) : coQuat(0.0f, -_from.z, _from.y, 0.0f));
		return coNormalize(coQuat(-_from.z, _from.y, _from.x, 0.0f));
	}
}
coFORCE_INLINE coQuat coRotationUnitVecs(const coVec3& _from, const coVec3& _to)
{
	coASSERT(coIsNormalized(_from));
	coASSERT(coIsNormalized(_to));
	return _coRotation(_from, _to, 1.0f);
}
coFORCE_INLINE coQuat coRotation(const coVec3& _from, const coVec3& _to)
{
	return _coRotation(_from, _to, coSquareRoot(coSquareLength(_from) * coSquareLength(_to)));
}
coFORCE_INLINE coQuat coInverse(const coQuat& _a) { return coBitCast<coQuat>(coBitCast<coInt32x4>(_a) ^ coInt32x4_SIGN_MASK_XYZ); }
coFORCE_INLINE coQuat coExp(const coQuat& _a)
{
	const float r = coSquareRoot(_a.x*_a.x + _a.y*_a.y + _a.z*_a.z);
	const float expW = coExp(_a.w);
	const float s = (r >= 0.00001f) ? expW * coSin(r) / r : 0.0f;
	return coQuat(s*_a.x, s*_a.y, s*_a.z, expW * coCos(r));
}
coFORCE_INLINE coQuat coLog(const coQuat& _a)
{
	coASSERT(false); // Check if the coAtan has the correct order of parameters.
	const float r = coSquareRoot(_a.x*_a.x + _a.y*_a.y + _a.z*_a.z);
	const float t = (r > 0.00001f) ? coAtan(r, _a.w) / r : 0.0f;
	return coQuat(t*_a.x, t*_a.y, t*_a.z, 0.5f * coLog(_a.x*_a.x + _a.y*_a.y + _a.z*_a.z + _a.w*_a.w));
}
coFORCE_INLINE coQuat coSyncWith(const coQuat& _a, const coQuat& _b)
{
	const coInt32x4 comp = coBitCast<coInt32x4>(coDot(coVec4(_a), coVec4(_b)) < coFloatx4_ZERO);
	return coBitCast<coQuat>(coBitCast<coInt32x4>(_a) ^ (comp & coBitCast<coInt32x4>(__m128_SIGN_MASK)));
}
coFORCE_INLINE coQuat operator*(const coQuat& a, const coQuat& b)
{
	if (true)
	{
		// Taken from: http://momchil-velikov.blogspot.nl/2013/10/fast-sse-quternion-multiplication.html
		const coFloatx4 abcd = a;
		const coFloatx4 xyzw = b;

		const coFloatx4 t0 = coBroadcastW(abcd);
		const coFloatx4 t1 = coShuffle<1, 0, 3, 2>(xyzw);

		const coFloatx4 t3 = coBroadcastX(abcd);
		const coFloatx4 t4 = coShuffle<2, 3, 0, 1>(xyzw);

		const coFloatx4 t5 = coBroadcastY(abcd);
		const coFloatx4 t6 = coShuffle<1, 3, 0, 2>(xyzw);

		// [d,d,d,d] * [z,w,x,y] = [dz,dw,dx,dy]
		const coFloatx4 m0 = t0 * t1;

		// [a,a,a,a] * [y,x,w,z] = [ay,ax,aw,az]
		const coFloatx4 m1 = t3 * t4;

		// [b,b,b,b] * [z,x,w,y] = [bz,bx,bw,by]
		const coFloatx4 m2 = t5 * t6;

		// [c,c,c,c] * [w,z,x,y] = [cw,cz,cx,cy]
		const coFloatx4 t7 = coBroadcastZ(abcd);
		const coFloatx4 t8 = coShuffle<1, 0, 2, 3>(xyzw);
		const coFloatx4 m3 = t7 * t8;

		// [dz,dw,dx,dy] + -[ay,ax,aw,az] = [dz+ay,dw-ax,dx+aw,dy-az]
		coFloatx4 e = coAddSub(m0, m1);

		// [dx+aw,dz+ay,dy-az,dw-ax]
		e = coShuffle<2, 0, 3, 1>(e);

		// [dx+aw,dz+ay,dy-az,dw-ax] + -[bz,bx,bw,by] = [dx+aw+bz,dz+ay-bx,dy-az+bw,dw-ax-by]
		e = coAddSub(e, m2);

		// [dz+ay-bx,dw-ax-by,dy-az+bw,dx+aw+bz]
		e = coShuffle<3, 1, 0, 2>(e);

		// [dz+ay-bx,dw-ax-by,dy-az+bw,dx+aw+bz] + -[cw,cz,cx,cy] = [dz+ay-bx+cw,dw-ax-by-cz,dy-az+bw+cx,dx+aw+bz-cy]
		e = coAddSub(e, m3);

		// [dw-ax-by-cz,dz+ay-bx+cw,dy-az+bw+cx,dx+aw+bz-cy]
		return coQuat(coShuffle<0, 1, 3, 2>(e));
	}
	
	else
	{
		// Implementation from JoltPhysics
		const coFloat lx = a.x;
		const coFloat ly = a.y;
		const coFloat lz = a.z;
		const coFloat lw = a.w;

		const coFloat rx = b.x;
		const coFloat ry = b.y;
		const coFloat rz = b.z;
		const coFloat rw = b.w;

		const coFloat x = lw * rx + lx * rw + ly * rz - lz * ry;
		const coFloat y = lw * ry - lx * rz + ly * rw + lz * rx;
		const coFloat z = lw * rz + lx * ry - ly * rx + lz * rw;
		const coFloat w = lw * rw - lx * rx - ly * ry - lz * rz;

		return coQuat(x, y, z, w);
	}
}

coFORCE_INLINE coQuat& operator*=(coQuat& this_, const coQuat& b)
{
	return this_ = this_ * b;
}
// coFORCE_INLINE coQuat operator*(const coQuat& _q, const coFloatx4& _xyzw)
// {
// 	coASSERT(false);
// 	return coMake_quat(coBitCast<coFloatx4>(_q) + _xyzw);
// }
coFORCE_INLINE coBool32x4 coIsValid(const coQuat& _this) { return coIsValid(coBitCast<coFloatx4>(_this)); }
void coSetupSquad(coQuat& _out0, coQuat& _out1, coQuat& _out3, const coQuat& _q0, const coQuat& _q1, const coQuat& _q2, const coQuat& _q3);
coFORCE_INLINE coVec3 coRotateVector(const coQuat& _this, const coVec3& _vec)
{
	// https://blog.molecular-matters.com/2013/05/24/a-faster-quaternion-vector-multiplication/

	const coVec3 q = coBitCast<coVec3>(_this);
	const coFloatx3 w = coFloatx3(coBroadcastW(_this));
	//return 2.0f * coDot(q, _vec) * q + (w*w - coDot(q, q)) * _vec + 2.0f * w * coCross(q, _vec);
	const coVec3 t = 2.0f * coCross(q, _vec);
	return _vec + w * t + coCross(q, t);
}

coFORCE_INLINE coVec3 coInverseRotateVector(const coQuat& _this, const coVec3& _vec)
{
	return coRotateVector(coInverse(_this), _vec);
}
