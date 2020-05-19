// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include <math/vector/coInt32x3.h>
#include "io/stream/coBinaryOutputStream.h"
#include "io/stream/coBinaryInputStream.h"

coFORCE_INLINE coInt32x3 coInt32x3_XMask() { return coBitCast<coInt32x3>(_mm_set_epi32(0, 0, 0, 0xffffffff)); }
coFORCE_INLINE coInt32x3 coInt32x3_YMask() { return coBitCast<coInt32x3>(_mm_set_epi32(0, 0, 0xffffffff, 0)); }
coFORCE_INLINE coInt32x3 coInt32x3_ZMask() { return coBitCast<coInt32x3>(_mm_set_epi32(0, 0xffffffff, 0, 0)); }
coFORCE_INLINE coInt32x3 coInt32x3_XYMask() { return coBitCast<coInt32x3>(_mm_set_epi32(0, 0, 0xffffffff, 0xffffffff)); }
coFORCE_INLINE coInt32x3 coInt32x3_XZMask() { return coBitCast<coInt32x3>(_mm_set_epi32(0, 0xffffffff, 0, 0xffffffff)); }
coFORCE_INLINE coInt32x3 coInt32x3_YZMask() { return coBitCast<coInt32x3>(_mm_set_epi32(0, 0xffffffff, 0xffffffff, 0)); }
coFORCE_INLINE coInt32x3 coInt32x3_XYZMask() { return coBitCast<coInt32x3>(_mm_set_epi32(0, 0xffffffff, 0xffffffff, 0xffffffff)); }
coFORCE_INLINE coInt32x3 operator& (const coInt32x3& _a, const coInt32x3& _b) { return coBitCast<coInt32x3>(_mm_and_si128(coBitCast<__m128i>(_a), coBitCast<__m128i>(_b))); }
coFORCE_INLINE coInt32x3 operator| (const coInt32x3& _a, const coInt32x3& _b) { return coBitCast<coInt32x3>(_mm_or_si128(coBitCast<__m128i>(_a), coBitCast<__m128i>(_b))); }
coFORCE_INLINE coInt32x3 operator^ (const coInt32x3& _a, const coInt32x3& _b) { return coBitCast<coInt32x3>(_mm_xor_si128(coBitCast<__m128i>(_a), coBitCast<__m128i>(_b))); }
coFORCE_INLINE coInt32x3 operator* (const coInt32x3& _a, const coInt32x3& _b) { return coBitCast<coInt32x3>(_mm_mullo_epi32(coBitCast<__m128i>(_a), coBitCast<__m128i>(_b))); }
coFORCE_INLINE coInt32x3 operator+ (const coInt32x3& _a, const coInt32x3& _b) { return coBitCast<coInt32x3>(_mm_add_epi32(coBitCast<__m128i>(_a), coBitCast<__m128i>(_b))); }
coFORCE_INLINE coInt32x3 operator- (const coInt32x3& _a, const coInt32x3& _b) { return coBitCast<coInt32x3>(_mm_sub_epi32(coBitCast<__m128i>(_a), coBitCast<__m128i>(_b))); }
coFORCE_INLINE coInt32x3 coAbs(const coInt32x3& _this) { return coBitCast<coInt32x3>(_mm_abs_epi32(coBitCast<__m128i>(_this))); }
coFORCE_INLINE coInt32x3 coMin(const coInt32x3& _a, const coInt32x3& _b) { return coBitCast<coInt32x3>(_mm_min_epi32(coBitCast<__m128i>(_a), coBitCast<__m128i>(_b))); }
coFORCE_INLINE coInt32x3 coMax(const coInt32x3& _a, const coInt32x3& _b) { return coBitCast<coInt32x3>(_mm_max_epi32(coBitCast<__m128i>(_a), coBitCast<__m128i>(_b))); }
coFORCE_INLINE coUint32 coConvertToMorton(const coInt32x3& a)
{
	// https://fgiesen.wordpress.com/2009/12/13/decoding-morton-codes/

	auto Part1By2 = [](coUint32 x)
	{
		x &= 0x000003ff;
		x = (x ^ (x << 16)) & 0xff0000ff;
		x = (x ^ (x << 8)) & 0x0300f00f;
		x = (x ^ (x << 4)) & 0x030c30c3;
		x = (x ^ (x << 2)) & 0x09249249;
		return x;
	};

	return (Part1By2(a.z) << 2) + (Part1By2(a.y) << 1) + Part1By2(a.x);
}

coFORCE_INLINE coInt32x3 coConvertFromMorton(coUint32 a)
{
	// https://fgiesen.wordpress.com/2009/12/13/decoding-morton-codes/

	auto Compact1By2 = [](coUint32 x)
	{
		x &= 0x09249249;                  // x = ---- 9--8 --7- -6-- 5--4 --3- -2-- 1--0
		x = (x ^ (x >> 2)) & 0x030c30c3; // x = ---- --98 ---- 76-- --54 ---- 32-- --10
		x = (x ^ (x >> 4)) & 0x0300f00f; // x = ---- --98 ---- ---- 7654 ---- ---- 3210
		x = (x ^ (x >> 8)) & 0xff0000ff; // x = ---- --98 ---- ---- ---- ---- 7654 3210
		x = (x ^ (x >> 16)) & 0x000003ff; // x = ---- ---- ---- ---- ---- --98 7654 3210
		return x;
	};

	return coInt32x3(Compact1By2(a), Compact1By2(a >> 1), Compact1By2(a >> 2));
}
coFORCE_INLINE coBinaryOutputStream& operator<<(coBinaryOutputStream& stream, const coInt32x3& a)
{
	stream.Write(&a, 12);
	return stream;
}
coFORCE_INLINE coBinaryInputStream& operator >> (coBinaryInputStream& stream, coInt32x3& a)
{
	stream.Read(&a, 12);
	return stream;
}
