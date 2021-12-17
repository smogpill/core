// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/shape/coDistance_f.h"

coVec4 coDistanceSegmentSegmentSquared4(const coVec3 p, const coVec3 d0,
	const coVec3 p02, const coVec3 d02,
	const coVec3 p12, const coVec3 d12,
	const coVec3 p22, const coVec3 d22,
	const coVec3 p32, const coVec3 d32,
	coVec4& s, coVec4& t);