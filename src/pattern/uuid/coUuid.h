// Copyright(c) 2020-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "container/map/coHashMapHash.h"
class coBinaryOutputStream;
class coBinaryInputStream;

class coUuid
{
public:
	coUuid() = default;
	coUuid(const coUuid& other) = default;
	coUuid(coUint64 low, coUint64 high) : low(low), high(high) {}
	coBool operator==(const coUuid& other) const { return low == other.low && high == other.high; }
	coBool operator!=(const coUuid& other) const { return low != other.low || high != other.high; }
	void Write(coBinaryOutputStream& stream) const;
	void Read(coBinaryInputStream& stream);
	coBool IsValid() const { return (low | high) != 0; }
	coBool IsInvalid() const { return (low | high) == 0; }

	coUint64 low = 0;
	coUint64 high = 0;

	static const coUuid invalid;
};

template <>
struct coHashMapHash<coUuid>
{
	coUint32 operator()(const coUuid& v) const;
};
