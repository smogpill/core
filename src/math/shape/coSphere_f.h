// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coSphere.h"
#include "../vector/coFloatx4_f.h"

coFORCE_INLINE void coSetCenter(coSphere& a, const coVec3& center) { a.centerAndRadius = coSelectXYZ(a.centerAndRadius, center); }
coFORCE_INLINE void coSetRadius(coSphere& a, const coFloatx4& radius) { a.centerAndRadius = coSelectXYZ(radius, a.centerAndRadius); }
