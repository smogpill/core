// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coAABox4.h"

const coAABox4 coAABox4::s_empty = coAABox4();
const coAABox4 coAABox4::s_zero = { coVec3x4(0.0f), coVec3x4(0.0f) };
