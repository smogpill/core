// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "container/array/coDynamicArray.h"
#include "math/vector/coVec2.h"
#include "coPolygon2.h"

void coMakeCircle(coPolygon2& out, const coFloat radius, coUint nbSegments);
void coMakeRectangle(coPolygon2& out, const coVec2& size);
void coBevel(coPolygon2& out, coFloat width, coUint nbSegments);