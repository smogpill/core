// Copyright(c) 2020-2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coUuid.h"
#include "io/stream/coBinaryInputStream.h"
#include "io/stream/coBinaryOutputStream.h"
#include "math/hash/coHash_f.h"

coUuid coMakeUuid();
coUuid coMakeUuid(const coChar*);
coFORCE_INLINE coBool coIsNull(const coUuid& u) { return u._u64[0] == 0 && u._u64[1] == 0; }

coFORCE_INLINE coUuid operator^(const coUuid& a, const coUuid& b)
{
	return coUuid(a._u64[0] ^ b._u64[0], a._u64[1] ^ b._u64[1]);
}

coFORCE_INLINE coUuid operator|(const coUuid& a, const coUuid& b)
{
	return coUuid(a._u64[0] | b._u64[0], a._u64[1] | b._u64[1]);
}

coFORCE_INLINE coUuid operator&(const coUuid& a, const coUuid& b)
{
	return coUuid(a._u64[0] & b._u64[0], a._u64[1] & b._u64[1]);
}

coFORCE_INLINE void coUuid::Write(coBinaryOutputStream& stream) const
{
	stream << _u64[0];
	stream << _u64[1];
}

coFORCE_INLINE void coUuid::Read(coBinaryInputStream& stream)
{
	stream >> _u64[0];
	stream >> _u64[1];
}

coFORCE_INLINE coUint32 coHashMapHash<coUuid>::operator()(const coUuid& v) const
{
	return coFastHash32(v._u64[1]) ^ coFastHash32(v._u64[0]);
}
