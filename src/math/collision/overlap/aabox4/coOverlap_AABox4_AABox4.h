// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../../../shape/coAabb_f.h"
#include "../../../shape/aabb/coAABox4_f.h"

coFORCE_INLINE coBool32x4 coOverlap_AABox4_AABox4(const coAABox4& a, const coAABox4& b)
{
	return coNot(coIsEmpty(coIntersect(a, b)));
}
