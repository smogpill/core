// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/transform/coTransform.h"
#include "math/vector/coBool32x4_f.h"
#include "math/vector/coFloatx3_f.h"
#include "math/quaternion/coQuat_f.h"

coBool32x4 coIsValid(const coTransform& _this);
coFloatx3 coInverseTransformDirection(const coTransform& _this, const coFloatx3& _dir);
