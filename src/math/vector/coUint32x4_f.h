// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coUint32x4.h"
#include "coBool32x4_f.h"

coFORCE_INLINE coUint32x4 operator^(const coUint32x4& a, const coUint32x4& b) { return coBitCast<coUint32x4>(_mm_xor_ps(coBitCast<__m128>(a), coBitCast<__m128>(b))); }
coFORCE_INLINE coUint32x4 operator&(const coUint32x4& a, const coUint32x4& b) { return coBitCast<coUint32x4>(_mm_and_ps(coBitCast<__m128>(a), coBitCast<__m128>(b))); }
coFORCE_INLINE coUint32x4 operator|(const coUint32x4& a, const coUint32x4& b) { return coBitCast<coUint32x4>(_mm_or_ps(coBitCast<__m128>(a), coBitCast<__m128>(b))); }
coFORCE_INLINE void coStore(const coUint32x4& v, coUint32* dest)
{
	_mm_storeu_si128(reinterpret_cast<__m128i*>(dest), coBitCast<__m128i>(v));
}

coFORCE_INLINE void coLoad(coUint32x4& v, const coUint32* source)
{
	v = coBitCast<coUint32x4>(_mm_loadu_si128(reinterpret_cast<const __m128i*>(source)));
}

coFORCE_INLINE coUint32x4 coSelect(const coUint32x4& a, const coUint32x4& b, const coBool32x4& mask)
{
	return coBitCast<coUint32x4>(b ^ coUint32x4(mask) & (a ^ b));
}

template <int X, int Y, int Z, int W>
coFORCE_INLINE coUint32x4 coShuffle(const coUint32x4& a, const coUint32x4& b) { return coBitCast<coUint32x4>(_mm_shuffle_ps(coBitCast<__m128>(a), coBitCast<__m128>(b), _MM_SHUFFLE(W, Z, Y, X))); }
template <int X, int Y, int Z, int W>
coFORCE_INLINE coUint32x4 coShuffle(const coUint32x4& a) { return coBitCast<coUint32x4>(_mm_shuffle_ps(coBitCast<__m128>(a), coBitCast<__m128>(a), _MM_SHUFFLE(W, Z, Y, X))); }
coFORCE_INLINE coUint32x4 coSplatX(const coUint32x4& a) { return coShuffle<0, 0, 0, 0>(a); }
coFORCE_INLINE coUint32x4 coSplatY(const coUint32x4& a) { return coShuffle<1, 1, 1, 1>(a); }
coFORCE_INLINE coUint32x4 coSplatZ(const coUint32x4& a) { return coShuffle<2, 2, 2, 2>(a); }
coFORCE_INLINE coUint32x4 coSplatW(const coUint32x4& a) { return coShuffle<3, 3, 3, 3>(a); }

coFORCE_INLINE coUint32x4 coSortTrueFirst(const coUint32x4& values, const coBool32x4& select)
{
	coUint32x4 out = coSelect(coShuffle<0, 1, 3, 3>(values), values, coSplatZ(select));
	out = coSelect(coShuffle<0, 2, 3, 3>(out), out, coSplatY(select));
	out = coSelect(coShuffle<1, 2, 3, 3>(out), out, coSplatX(select));
	return out;
}
