// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <math/shape/coAabb.h>

coBool coIntersect(const coAabb& this_, const coVec3& point);

coFORCE_INLINE coBool coIntersect(const coAabb& this_, const coVec3& point)
{
	return coAreAllTrue(point >= this_.min && point <= this_.max);
}

coFORCE_INLINE coAabb operator/ (const coAabb& a, const coFloatx3& b) { return coAabb(a.min/b, a.max/b); }
coFORCE_INLINE coAabb& operator/= (coAabb& this_, const coFloatx3& b) { this_.min /= b; this_.max /= b; return this_; }
coFORCE_INLINE coAabb operator* (const coAabb& a, const coFloatx3& b) { return coAabb(a.min*b, a.max*b); }
coFORCE_INLINE coAabb& operator*= (coAabb& this_, const coFloatx3& b) { this_.min *= b; this_.max *= b; return this_; }
