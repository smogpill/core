// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
class coBinaryOutputStream;
class coBinaryInputStream;

class coUuid
{
public:
	coUuid() = default;
	coUuid(coUint64 low, coUint64 high) : low(low), high(high) {}
	coBool operator==(const coUuid& other) const { return low == other.low && high == other.high; }
	coBool operator!=(const coUuid& other) const { return low != other.low || high != other.high; }
	void Write(coBinaryOutputStream& stream) const;
	void Read(coBinaryInputStream& stream);

	coUint64 low = 0;
	coUint64 high = 0;
};
