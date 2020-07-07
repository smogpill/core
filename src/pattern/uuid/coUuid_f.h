// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coUuid.h"
#include "io/stream/coBinaryInputStream.h"
#include "io/stream/coBinaryOutputStream.h"

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
