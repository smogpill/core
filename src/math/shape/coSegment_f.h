// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/shape/coSegment.h"
#include "math/vector/coVec3_f.h"

coFORCE_INLINE coFloatx4 coSquareLength(const coSegment& s) { return coSquareLength(s.p1 - s.p0); }
coFORCE_INLINE coFloatx4 coLength(const coSegment& s) { return coLength(s.p1 - s.p0); }
