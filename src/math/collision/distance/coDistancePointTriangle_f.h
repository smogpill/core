// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/vector/coVec3.h"

coVec3 coClosestPtPointTriangle(const coVec3& p, const coVec3& a, const coVec3& b, const coVec3& c, coFloat& s, coFloat& t);