// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/scalar/float_f.h"
#include "math/vector/Floatx4_f.h"
#include "math/vector/Int32x4_f.h"
#include "math/quaternion/Quat.h"
#include "lang/cppExtensions.h"

namespace co
{
	coFORCE_INLINE const Floatx4& make_floatx4(const Quat& _a) { return bitCast<Floatx4>(_a); }
	coFORCE_INLINE const Quat& make_quat(const Floatx4& _a) { return bitCast<Quat>(_a); }
	coFORCE_INLINE Quat make_quat(float _x, float _y, float _z, float _w) { return make_quat(make_floatx4( _x, _y, _z, _w )); }
	coFORCE_INLINE Quat conjugate(const Quat& _a) { return bitCast<Quat>(bitCast<Int32x4>(_a) ^ bitCast<Int32x4>(__m128_MASK_XYZ)); }
	coFORCE_INLINE Quat exp(const Quat& _a)
	{
		const float r = sqrt(_a.x*_a.x + _a.y*_a.y + _a.z*_a.z);
		const float expW = exp(_a.w);
		const float s = (r >= 0.00001f) ? expW * sin(r) / r : 0.0f;
		return make_quat(s*_a.x, s*_a.y, s*_a.z, expW * cos(r));
	}
	coFORCE_INLINE Quat ln(const Quat& _a)
	{
		const float r = sqrt(_a.x*_a.x + _a.y*_a.y + _a.z*_a.z);
		const float t = (r > 0.00001f) ? atan(r, _a.w) / r : 0.0f;
		return make_quat(t*_a.x, t*_a.y, t*_a.z, 0.5f * ln(_a.x*_a.x + _a.y*_a.y + _a.z*_a.z + _a.w*_a.w));
	}
	coFORCE_INLINE Quat syncWith(const Quat& _a, const Quat& _b)
	{
		const Int32x4 comp = bitCast<Int32x4>(dot(make_floatx4(_a), make_floatx4(_b)) < Floatx4_ZERO);
		return bitCast<Quat>(bitCast<Int32x4>(_a) ^ (comp & bitCast<Int32x4>(__m128_SIGN_MASK)));
	}
	coFORCE_INLINE Quat operator*(const Quat& _q0, const Quat& _q1)
	{
		// Wa * Wb - XYZa.XYZb + Wa*XYZb + Wb*XYZa + XYZa^XYZb
		// X = - Wa*Xb - Za*Yb - Ya*Zb + Xa*Wb
		// Y = - Za*Xb - Wa*Yb - Xa*Zb + Ya*Wb
		// Z = - Ya*Xb - Xa*Yb - Wa*Zb + Za*Wb
		// W = - Xa*Xb - Ya*Yb - Za*Zb + Wa*Wb
		const Floatx4 a = bitCast<Floatx4>(_q0);
		const Floatx4 b = bitCast<Floatx4>(_q1);
		const Floatx4 Xb = broadcastX(b);
		const Floatx4 Yb = broadcastY(b);
		const Floatx4 Zb = broadcastZ(b);
		const Floatx4 Wb = broadcastW(b);
		const Floatx4 WZYXa = shuffle<0, 1, 2, 3>(a, a);
		const Floatx4 ZWXYa = shuffle<1, 0, 3, 2>(a, a);
		const Floatx4 YXWZa = shuffle<2, 3, 0, 1>(a, a);
		const Floatx4 XCol = WZYXa * Xb;
		const Floatx4 YCol = ZWXYa * Yb;
		const Floatx4 ZCol = YXWZa * Zb;
		const Floatx4 WCol = a * Wb;
		return make_quat(WCol - (ZCol + XCol + YCol));
	}
	coFORCE_INLINE Quat operator+(const Quat& _q0, const Quat& _q1)
	{
		return make_quat(bitCast<Floatx4>(_q0) + bitCast<Floatx4>(_q1));
	}
	coFORCE_INLINE Quat operator*(const Quat& _q, const Floatx4& _xyzw)
	{
		coASSERT(false);
		return make_quat(bitCast<Floatx4>(_q) + _xyzw);
	}
	void squadSetup(Quat& _out0, Quat& _out1, Quat& _out3, const Quat& _q0, const Quat& _q1, const Quat& _q2, const Quat& _q3);
}
