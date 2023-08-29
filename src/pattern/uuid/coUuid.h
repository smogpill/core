// Copyright(c) 2020-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "container/map/coHashMapHash.h"
#include <lang/reflect/coTypeDecl.h>
class coBinaryOutputStream;
class coBinaryInputStream;

class coUuid
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coUuid);
public:
	coUuid() = default;
	coUuid(const coUuid& other) = default;
	coUuid(coUint64 low, coUint64 high) : _u64{ low, high } {}
	coUuid(coUint32 user0, coUint32 user1, coUint32 user2, coUint32 type) : _u64{ (coUint64(user1) << 32) | coUint64(user0), (coUint64(type) << 32) | coUint64(user2) } {}
	void SetUser(coUint32 user0, coUint32 user1, coUint32 user2) { _u32[0] = user0; _u32[1] = user1; _u32[2] = user2; }
	void SetUser(const coUuid& user) { _u64[0] = user._u64[0]; _u32[0] = user._u32[0]; }
	void SetType(coUint32 type) { _u32[3] = type; }
	coBool operator==(const coUuid& other) const { return _u64[0] == other._u64[0] && _u64[1] == other._u64[1]; }
	coBool operator!=(const coUuid& other) const { return _u64[0] != other._u64[0] || _u64[1] != other._u64[1]; }
	void Write(coBinaryOutputStream& stream) const;
	void Read(coBinaryInputStream& stream);
	coBool IsValid() const { return (_u64[0] | _u64[1]) != 0; }
	coBool IsInvalid() const { return (_u64[0] | _u64[1]) == 0; }

	union
	{
		coUint64 _u64[2] = {};
		coUint32 _u32[4];
	};

	static const coUuid invalid;
};

template <>
struct coHashMapHash<coUuid>
{
	coUint32 operator()(const coUuid& v) const;
};
