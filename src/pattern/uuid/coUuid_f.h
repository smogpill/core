// Copyright(c) 2020-2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coUuid.h"
#include "io/stream/coBinaryInputStream.h"
#include "io/stream/coBinaryOutputStream.h"
#include "math/hash/coHash_f.h"

coUuid coMakeUuid();
coUuid coMakeUuid(const coChar*);

coFORCE_INLINE void coUuid::Write(coBinaryOutputStream& stream) const
{
	stream << low;
	stream << high;
}

coFORCE_INLINE void coUuid::Read(coBinaryInputStream& stream)
{
	stream >> low;
	stream >> high;
}

coFORCE_INLINE coUint32 coHashMapHash<coUuid>::operator()(const coUuid& v) const
{
	return coFastHash32(v.high) ^ coFastHash32(v.low);
}
