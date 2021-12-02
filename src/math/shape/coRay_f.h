// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coRay.h"
#include "../vector/coVec3_f.h"

coFORCE_INLINE coFloatx4 coSquareDistance(const coInfiniteRay& ray, const coVec3& point)
{
	return coSquareLength(coCross(ray.GetDir(), point - ray.origin));
}

coFORCE_INLINE coFloatx4 coDistance(const coInfiniteRay& ray, const coVec3& point)
{
	return coLength(coCross(ray.GetDir(), point - ray.origin));
}
