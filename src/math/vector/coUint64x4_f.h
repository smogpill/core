// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coUint64x4.h"

/// Creates a full mask without having to rely on a constant
coFORCE_INLINE coUint64x4 coFullMask(const coUint64x4& m)
{
	return coBitCast<coUint64x4>(_mm256_cmpeq_epi32(coBitCast<__m256i>(m), coBitCast<__m256i>(m)));
}

coFORCE_INLINE coUint64x4 operator^(const coUint64x4& a, const coUint64x4& b)
{
	return coBitCast<coUint64x4>(_mm256_xor_si256(coBitCast<__m256i>(a), coBitCast<__m256i>(b)));
}

coFORCE_INLINE coUint64x4 operator&(const coUint64x4& a, const coUint64x4& b)
{
	return coBitCast<coUint64x4>(_mm256_and_si256(coBitCast<__m256i>(a), coBitCast<__m256i>(b)));
}

coFORCE_INLINE coUint64x4 operator|(const coUint64x4& a, const coUint64x4& b)
{
	return coBitCast<coUint64x4>(_mm256_or_si256(coBitCast<__m256i>(a), coBitCast<__m256i>(b)));
}

coFORCE_INLINE coUint64x4 operator~(const coUint64x4& m)
{
	return coBitCast<coUint64x4>(_mm256_xor_si256(coBitCast<__m256i>(m), coBitCast<__m256i>(coFullMask(m))));
}
