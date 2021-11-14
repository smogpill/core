// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "container/array/coDynamicArray.h"
#include "math/vector/coVec2.h"
#include "math/vector/coVec3.h"

coDynamicArray<coVec2> coBevel(const coDynamicArray<coVec2>& curve, coFloat width, coUint nbSegments, coBool loop);
coDynamicArray<coVec3> coBevel(const coDynamicArray<coVec3>& curve, coFloat width, coUint nbSegments, coBool loop);