// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "coBool32x3_f.h"
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
/// Creates a full mask without having to rely on a constant
coFORCE_INLINE coInt32x3 coFullMask(const coInt32x3& m)
{
	return coBitCast<coInt32x3>(_mm_cmpeq_epi32(coBitCast<__m128i>(m), coBitCast<__m128i>(m)));
}
coFORCE_INLINE coBool32x3 operator== (const coInt32x3& a, const coInt32x3& b) { return coBitCast<coBool32x3>(_mm_cmpeq_epi32(coBitCast<__m128i>(a), coBitCast<__m128i>(b))); }
coFORCE_INLINE coBool32x3 operator!= (const coInt32x3& a, const coInt32x3& b) { return coNot(coBitCast<coBool32x3>(_mm_cmpeq_epi32(coBitCast<__m128i>(a), coBitCast<__m128i>(b)))); }
coFORCE_INLINE coInt32x3 operator& (const coInt32x3& a, const coInt32x3& b) { return coBitCast<coInt32x3>(_mm_and_si128(coBitCast<__m128i>(a), coBitCast<__m128i>(b))); }
coFORCE_INLINE coInt32x3 operator| (const coInt32x3& a, const coInt32x3& b) { return coBitCast<coInt32x3>(_mm_or_si128(coBitCast<__m128i>(a), coBitCast<__m128i>(b))); }
coFORCE_INLINE coInt32x3 operator^ (const coInt32x3& a, const coInt32x3& b) { return coBitCast<coInt32x3>(_mm_xor_si128(coBitCast<__m128i>(a), coBitCast<__m128i>(b))); }
coFORCE_INLINE coInt32x3 operator~(const coInt32x3& m)
{
	return coBitCast<coInt32x3>(_mm_xor_si128(coBitCast<__m128i>(m), coBitCast<__m128i>(coFullMask(m))));
}
coFORCE_INLINE coInt32x3 operator* (const coInt32x3& a, const coInt32x3& b) { return coBitCast<coInt32x3>(_mm_mullo_epi32(coBitCast<__m128i>(a), coBitCast<__m128i>(b))); }
coFORCE_INLINE coInt32x3 operator/ (const coInt32x3& a, const coInt32x3& b) { return coInt32x3(a.x / b.x, a.y / b.y, a.z / b.z); }
coFORCE_INLINE coInt32x3 operator+ (const coInt32x3& a, const coInt32x3& b) { return coBitCast<coInt32x3>(_mm_add_epi32(coBitCast<__m128i>(a), coBitCast<__m128i>(b))); }
coFORCE_INLINE coInt32x3 operator- (const coInt32x3& a, const coInt32x3& b) { return coBitCast<coInt32x3>(_mm_sub_epi32(coBitCast<__m128i>(a), coBitCast<__m128i>(b))); }
coFORCE_INLINE coInt32x3 operator<< (const coInt32x3& a, const coInt shift) { return coBitCast<coInt32x3>(_mm_slli_epi32(coBitCast<__m128i>(a), shift)); }
coFORCE_INLINE coInt32x3 operator>> (const coInt32x3& a, const coInt shift) { return coBitCast<coInt32x3>(_mm_srli_epi32(coBitCast<__m128i>(a), shift)); }
coFORCE_INLINE void operator|= (coInt32x3& a, const coInt32x3& b) { a = coBitCast<coInt32x3>(_mm_or_si128(coBitCast<__m128i>(a), coBitCast<__m128i>(b))); }
coFORCE_INLINE void operator&= (coInt32x3& a, const coInt32x3& b) { a = coBitCast<coInt32x3>(_mm_and_si128(coBitCast<__m128i>(a), coBitCast<__m128i>(b))); }
coFORCE_INLINE void operator<<= (coInt32x3& a, const coInt shift) { a = coBitCast<coInt32x3>(_mm_slli_epi32(coBitCast<__m128i>(a), shift)); }
coFORCE_INLINE void operator>>= (coInt32x3& a, const coInt shift) { a = coBitCast<coInt32x3>(_mm_srli_epi32(coBitCast<__m128i>(a), shift)); }
coFORCE_INLINE coInt32x3 coAbs(const coInt32x3& this_) { return coBitCast<coInt32x3>(_mm_abs_epi32(coBitCast<__m128i>(this_))); }
coFORCE_INLINE coInt32x3 coMin(const coInt32x3& a, const coInt32x3& b) { return coBitCast<coInt32x3>(_mm_min_epi32(coBitCast<__m128i>(a), coBitCast<__m128i>(b))); }
coFORCE_INLINE coInt32x3 coMax(const coInt32x3& a, const coInt32x3& b) { return coBitCast<coInt32x3>(_mm_max_epi32(coBitCast<__m128i>(a), coBitCast<__m128i>(b))); }
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
coDynamicString& operator<<(coDynamicString& s, const coInt32x3& a);