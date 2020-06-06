// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coSphere.h"
#include "../vector/coFloatx4_f.h"

//: centerAndRadius(coSelectXYZ(center, radius)) {}

coSphere::coSphere(const coVec3& center, const coFloatx4& radius)
	: centerAndRadius(coSelectXYZ(coBitCast<coFloatx4>(center), radius))
{
}
