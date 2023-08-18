// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../../vector/coFloatx4.h"
#include "../coAabb.h"

class alignas(16) coAABox4
{
public:
	static const coAABox4 s_empty;
	static const coAABox4 s_zero;
	coAABox4() = default;
	coAABox4(coNullPtr) : _minX(nullptr), _minY(nullptr), _minZ(nullptr), 
		_maxX(nullptr), _maxY(nullptr), _maxZ(nullptr) {}
	coAABox4(const coAabb& aabox) 
		: _minX(coBroadcastX(aabox.min)), _minY(coBroadcastY(aabox.min)), _minZ(coBroadcastZ(aabox.min)),
		_maxX(coBroadcastX(aabox.max)), _maxY(coBroadcastY(aabox.max)), _maxZ(coBroadcastZ(aabox.max)) {}
	coAABox4(const coFloatx4& minX, const coFloatx4& minY, const coFloatx4& minZ,
		const coFloatx4& maxX, const coFloatx4& maxY, const coFloatx4& maxZ) 
		: _minX(minX), _minY(minY), _minZ(minZ), _maxX(maxX), _maxY(maxY), _maxZ(maxZ) {}

	coFORCE_INLINE coAabb operator[](coUint index) const { return coAabb(coVec3(_minX.x, _minY.x, _minZ.x), coVec3(_maxX.x, _maxY.x, _maxZ.x)); }

	coFloatx4 _minX = coFloatx4(1e30f);
	coFloatx4 _minY = coFloatx4(1e30f);
	coFloatx4 _minZ = coFloatx4(1e30f);
	coFloatx4 _maxX = coFloatx4(-1e30f);
	coFloatx4 _maxY = coFloatx4(-1e30f);
	coFloatx4 _maxZ = coFloatx4(-1e30f);
};
