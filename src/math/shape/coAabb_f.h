// Copyright(c) 2016-2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <math/shape/coAabb.h>
#include <math/vector/coBool32x3.h>
#include "../vector/coVec3_f.h"

coFORCE_INLINE coVec3 coGetSize(const coAabb& this_) { return this_.max - this_.min; }
coFORCE_INLINE coVec3 coGetCenter(const coAabb& this_) { return (this_.max + this_.min) * 0.5f; }
coFORCE_INLINE coAabb coIntersect(const coAabb& a, const coAabb& b) { return coAabb(coMax(a.min, b.min), coMin(a.max, b.max)); }
coFORCE_INLINE coBool32x3 coIsEmpty(const coAabb& a) { return a.max < a.min; }
coFORCE_INLINE coAabb coMerge(const coAabb& a, const coAabb& b) { return coAabb(coMin(a.min, b.min), coMax(a.max, b.max)); }
coFORCE_INLINE coAabb coMerge(const coAabb& a, const coVec3& p) { return coAabb(coMin(a.min, p), coMax(a.max, p)); }
coFORCE_INLINE coAabb coGrow(const coAabb& this_, const coFloatx3& offset) { return coAabb(this_.min - offset, this_.max + offset); }
coFORCE_INLINE coAabb operator/ (const coAabb& a, const coFloatx3& b) { return coAabb(a.min/b, a.max/b); }
coFORCE_INLINE coAabb& operator/= (coAabb& this_, const coFloatx3& b) { this_.min /= b; this_.max /= b; return this_; }
coFORCE_INLINE coAabb operator* (const coAabb& a, const coFloatx3& b) { return coAabb(a.min*b, a.max*b); }
coFORCE_INLINE coAabb& operator*= (coAabb& this_, const coFloatx3& b) { this_.min *= b; this_.max *= b; return this_; }
coFORCE_INLINE coAabb& operator+= (coAabb& this_, const coFloatx3& b) { this_.min += b; this_.max += b; return this_; }
coFORCE_INLINE coAabb& operator-= (coAabb& this_, const coFloatx3& b) { this_.min -= b; this_.max -= b; return this_; }
coFORCE_INLINE coAabb operator- (const coAabb& a, const coFloatx3& b) { return coAabb(a.min - b, a.max - b); }
coFORCE_INLINE coAabb operator+ (const coAabb& a, const coFloatx3& b) { return coAabb(a.min + b, a.max + b); }
