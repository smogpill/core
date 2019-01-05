// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <math/shape/coAabb.h>

coBool coIntersect(const coAabb& _this, const coVec3& _point);

coFORCE_INLINE coBool coIntersect(const coAabb& _this, const coVec3& _point)
{
	return coAreAllTrue(_point >= _this.min && _point <= _this.max);
}
