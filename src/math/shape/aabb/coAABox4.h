// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../../vector/coVec3x4.h"
#include "../coAabb.h"

class alignas(16) coAABox4
{
public:
	static const coAABox4 s_empty;
	static const coAABox4 s_zero;
	coAABox4() = default;
	coAABox4(coNullPtr) : _min(nullptr), _max(nullptr) {}
	coAABox4(const coAabb& aabox) 
		: _min(coVec3x4(coBroadcastX(aabox.min), coBroadcastY(aabox.min), coBroadcastZ(aabox.min))),
		_max(coVec3x4(coBroadcastX(aabox.max), coBroadcastY(aabox.max), coBroadcastZ(aabox.max))) {}
	coAABox4(const coVec3x4& min, const coVec3x4& max) 
		: _min(min), _max(max) {}

	coFORCE_INLINE coAabb operator[](coUint index) const { return coAabb(coVec3(_minX.x, _minY.x, _minZ.x), coVec3(_maxX.x, _maxY.x, _maxZ.x)); }

	coVec3x4 _min = coVec3x4(1e30f);
	coVec3x4 _max = coVec3x4(-1e30f);
};
