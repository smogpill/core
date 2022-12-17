// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coUint32x2.h"

coFORCE_INLINE coBool operator== (const coUint32x2& a, const coUint32x2& b) { return a.x == b.x && a.y == b.y; }
coFORCE_INLINE coBool operator!= (const coUint32x2& a, const coUint32x2& b) { return a.x != b.x || a.y != b.y; }