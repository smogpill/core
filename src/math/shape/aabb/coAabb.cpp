// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "../coAabb.h"

const coAabb coAabb::empty = coAabb(coVec3(1e30f), coVec3(-1e30f));
const coAabb coAabb::zero = coAabb(coVec3(0.0f), coVec3(0.0f));
