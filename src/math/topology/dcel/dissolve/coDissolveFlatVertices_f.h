// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/vector/coVec3.h"
#include <container/array/coArray.h>
class coDCEL;

// Merge two edges into one when they are almost colinear
// The tolerance is currently arbitrary and related to the cosinus of the angle, and subject to change later on.
void coDissolveFlatVertices(coDCEL& dcel, coFloat tolerance);