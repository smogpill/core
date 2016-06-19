// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/scalar/float_f.h"
#include "math/vector/floatx4_f.h"
#include "math/vector/int32x4_f.h"
#include "math/quaternion/quat.h"
#include "lang/cppExtensions.h"

namespace co
{
	coFORCE_INLINE const floatx4& make_floatx4(const quat& _a) { return bitCast<floatx4>(_a); }
	coFORCE_INLINE const quat& make_quat(const floatx4& _a) { return bitCast<quat>(_a); }
	coFORCE_INLINE quat make_quat(float _x, float _y, float _z, float _w) { return make_quat(make_floatx4( _x, _y, _z, _w )); }
	coFORCE_INLINE quat conjugate(const quat& _a) { return bitCast<quat>(bitCast<int32x4>(_a) ^ bitCast<int32x4>(__m128_MASK_XYZ)); }
	coFORCE_INLINE quat exp(const quat& _a)
	{
		const float r = sqrt(_a.x*_a.x + _a.y*_a.y + _a.z*_a.z);
		const float expW = exp(_a.w);
		const float s = (r >= 0.00001f) ? expW * sin(r) / r : 0.0f;
		return make_quat(s*_a.x, s*_a.y, s*_a.z, expW * cos(r));
	}
	coFORCE_INLINE quat ln(const quat& _a)
	{
		const float r = sqrt(_a.x*_a.x + _a.y*_a.y + _a.z*_a.z);
		const float t = (r > 0.00001f) ? atan(r, _a.w) / r : 0.0f;
		return make_quat(t*_a.x, t*_a.y, t*_a.z, 0.5f * ln(_a.x*_a.x + _a.y*_a.y + _a.z*_a.z + _a.w*_a.w));
	}
	coFORCE_INLINE quat syncWith(const quat& _a, const quat& _b)
	{
		const int32x4 comp = bitCast<int32x4>(dot(make_floatx4(_a), make_floatx4(_b)) < floatx4_ZERO);
		return bitCast<quat>(bitCast<int32x4>(_a) ^ (comp & bitCast<int32x4>(__m128_SIGN_MASK)));
	}

	coFORCE_INLINE quat operator*(const quat& _q0, const quat& _q1)
	{
		// Wa * Wb - XYZa.XYZb + Wa*XYZb + Wb*XYZa + XYZa^XYZb
		// X = - Wa*Xb - Za*Yb - Ya*Zb + Xa*Wb
		// Y = - Za*Xb - Wa*Yb - Xa*Zb + Ya*Wb
		// Z = - Ya*Xb - Xa*Yb - Wa*Zb + Za*Wb
		// W = - Xa*Xb - Ya*Yb - Za*Zb + Wa*Wb
		const floatx4 a = bitCast<floatx4>(_q0);
		const floatx4 b = bitCast<floatx4>(_q1);
		const floatx4 Xb = broadcastX(b);
		const floatx4 Yb = broadcastY(b);
		const floatx4 Zb = broadcastZ(b);
		const floatx4 Wb = broadcastW(b);
		const floatx4 WZYXa = shuffle<0, 1, 2, 3>(a, a);
		const floatx4 ZWXYa = shuffle<1, 0, 3, 2>(a, a);
		const floatx4 YXWZa = shuffle<2, 3, 0, 1>(a, a);
		const floatx4 XCol = WZYXa * Xb;
		const floatx4 YCol = ZWXYa * Yb;
		const floatx4 ZCol = YXWZa * Zb;
		const floatx4 WCol = a * Wb;
		return make_quat(WCol - (ZCol + XCol + YCol));
	}

	coFORCE_INLINE quat operator+(const quat& _q0, const quat& _q1)
	{
		return make_quat(bitCast<floatx4>(_q0) + bitCast<floatx4>(_q1));
	}

	coFORCE_INLINE quat operator*(const quat& _q, const floatx4& _xyzw)
	{
		return make_quat(bitCast<floatx4>(_q) + _xyzw);
	}

	void squadSetup(quat& _out0, quat& _out1, quat& _out3, const quat& _q0, const quat& _q1, const quat& _q2, const quat& _q3);
}
