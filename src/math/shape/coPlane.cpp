// Copyright(c) 2016-2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "math/shape/coPlane.h"
#include "math/vector/coFloatx4_f.h"
#include "math/vector/coVec3_f.h"
#include "math/shape/triangle/coTriangle_f.h"

coPlane::coPlane(const coVec3& normal, const coVec3& point)
{
	normalAndDistance = coSelectXYZ(reinterpret_cast<const coFloatx4&>(normal), -coBroadcastX(coDot(normal, point)));
}

coPlane::coPlane(const coVec3& a, const coVec3& b, const coVec3& c)
	: coPlane(coGetNormal(a, b, c), a)
{
}
