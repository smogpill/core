// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma 
#include "coVec3x4.h"
#include "coFloatx4_f.h"
#include "coBool32x3x4.h"

coFORCE_INLINE coBool32x3x4 operator<(const coVec3x4& a, const coVec3x4& b) { const coBool32x3x4(a.x < b.x, a.y < b.y, a.z < b.z); }
coFORCE_INLINE coBool32x3x4 operator<(const coVec3x4& a, const coFloatx4& b) { const coBool32x3x4(a.x < b, a.y < b, a.z < b); }
coFORCE_INLINE coBool32x3x4 operator<=(const coVec3x4& a, const coVec3x4& b) { const coBool32x3x4(a.x <= b.x, a.y <= b.y, a.z <= b.z); }
coFORCE_INLINE coBool32x3x4 operator<=(const coVec3x4& a, const coFloatx4& b) { const coBool32x3x4(a.x <= b, a.y <= b, a.z <= b); }
coFORCE_INLINE coVec3x4 operator*(const coVec3x4& a, const coFloatx4& b) { return coVec3x4(a.x * b, a.y * b, a.z * b); }
coFORCE_INLINE coVec3x4 operator/(const coVec3x4& a, const coFloatx4& b) { return coVec3x4(a.x / b, a.y / b, a.z / b); }
coFORCE_INLINE coVec3x4 operator+(const coVec3x4& a, const coFloatx4& b) { return coVec3x4(a.x + b, a.y + b, a.z + b); }
coFORCE_INLINE coVec3x4 operator+(const coVec3x4& a, const coVec3x4& b) { return coVec3x4(a.x + b.x, a.y + b.y, a.z + b.z); }
coFORCE_INLINE coVec3x4 operator-(const coVec3x4& a, const coFloatx4& b) { return coVec3x4(a.x - b, a.y - b, a.z - b); }
coFORCE_INLINE coVec3x4 operator-(const coVec3x4& a, const coVec3x4& b) { return coVec3x4(a.x - b.x, a.y - b.y, a.z - b.z); }
coFORCE_INLINE coVec3x4 coAbs(const coVec3x4& a) { return coVec3x4(coAbs(a.x), coAbs(a.y), coAbs(a.z)); }
coFORCE_INLINE coVec3x4 coMin(const coVec3x4& a, const coVec3x4& b) { return coVec3x4(coMin(a.x, b.x), coMin(a.y, b.y), coMin(a.z, b.z)); }
coFORCE_INLINE coVec3x4 coMin(const coFloatx4& a, const coVec3x4& b) { return coVec3x4(coMin(a, b.x), coMin(a, b.y), coMin(a, b.z)); }
coFORCE_INLINE coVec3x4 coMin(const coVec3x4& a, const coFloatx4& b) { return coVec3x4(coMin(a.x, b), coMin(a.y, b), coMin(a.z, b)); }
coFORCE_INLINE coVec3x4 coMax(const coVec3x4& a, const coVec3x4& b) { return coVec3x4(coMax(a.x, b.x), coMax(a.y, b.y), coMax(a.z, b.z)); }
coFORCE_INLINE coVec3x4 coMax(const coFloatx4& a, const coVec3x4& b) { return coVec3x4(coMax(a, b.x), coMax(a, b.y), coMax(a, b.z)); }
coFORCE_INLINE coVec3x4 coMax(const coVec3x4& a, const coFloatx4& b) { return coVec3x4(coMax(a.x, b), coMax(a.y, b), coMax(a.z, b)); }
coFORCE_INLINE coFloatx4 coMin(const coVec3x4& a) { return coMin(a.x, coMin(a.y, a.z)); }
coFORCE_INLINE coFloatx4 coMax(const coVec3x4& a) { return coMax(a.x, coMax(a.y, a.z)); }
coFORCE_INLINE coFloatx4 coSquareLength(const coVec3x4& a) { return a.x * a.x + a.y * a.y + a.z * a.z; }
coFORCE_INLINE coFloatx4 coLength(const coVec3x4& a) { return coSquareRoot(coSquareLength(a)); }
