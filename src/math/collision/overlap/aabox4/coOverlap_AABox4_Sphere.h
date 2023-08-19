// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../../../shape/coAabb_f.h"
#include "../../../shape/coSphere.h"
#include "../../../shape/aabb/coAABox4_f.h"

coFORCE_INLINE coBool32x4 coOverlapSolidSolid(const coAABox4& a, const coSphere& b)
{
	const coFloatx4 half = coFloatx4(0.5f);
	const coVec3x4 center = (a._min + a._max) * half;
	const coVec3x4 d = coAbs(coSplatX(b.centerAndRadius) - center) - (a._max - a._min) * half;
	const coVec3x4 distance = coLength(coMax(d, coFloatx4(0.0f))) + coMin(coMax(d), coFloatx4(0.0f));
	return distance <= coSplatW(b.centerAndRadius);
}
