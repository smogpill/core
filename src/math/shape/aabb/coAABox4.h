// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../../vector/coVec3x4.h"
#include "../../vector/coVec3_f.h"
#include "../coAabb.h"

class alignas(16) coAABox4
{
public:
	static const coAABox4 s_empty;
	static const coAABox4 s_zero;
	coAABox4() = default;
	coAABox4(coNullPtr) : _min(nullptr), _max(nullptr) {}
	coAABox4(const coAabb& aabox) 
		: _min(coSplatX(coFloatx4(aabox.min)), coSplatY(coFloatx4(aabox.min)), coSplatZ(coFloatx4(aabox.min))),
		_max(coSplatX(coFloatx4(aabox.max)), coSplatY(coFloatx4(aabox.max)), coSplatZ(coFloatx4(aabox.max))) {}
	coAABox4(const coVec3x4& min, const coVec3x4& max) 
		: _min(min), _max(max) {}

	// Return value is const to avoid mistakes because of missing non-const operator[]
	coFORCE_INLINE const coAabb operator[](coUint index) const { return coAabb(coVec3(_min.x[index], _min.y[index], _min.z[index]), coVec3(_max.x[index], _max.y[index], _max.z[index])); }

	coVec3x4 _min = coVec3x4(1e30f);
	coVec3x4 _max = coVec3x4(-1e30f);
};
