// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coAABox4.h"
#include "../coAabb.h"
#include "../../vector/coFloatx4_f.h"

coFORCE_INLINE coAABox4 coIntersect(const coAABox4& a, const coAabb& b)
{
	return coAABox4(
		coVec3x4(
			coMax(a._min.x, coSplatX(coFloatx4(b.min))),
			coMax(a._min.y, coSplatY(coFloatx4(b.min))),
			coMax(a._min.z, coSplatZ(coFloatx4(b.min)))),
		coVec3x4(
			coMin(a._max.x, coSplatX(coFloatx4(b.max))),
			coMin(a._max.y, coSplatY(coFloatx4(b.max))),
			coMin(a._max.z, coSplatZ(coFloatx4(b.max))))
	);
}

coFORCE_INLINE coAABox4 coIntersect(const coAABox4& a, const coAABox4& b)
{
	return coAABox4(
		coVec3x4(
			coMax(a._min.x, b._min.x),
			coMax(a._min.y, b._min.y),
			coMax(a._min.z, b._min.z)),
		coVec3x4(
			coMin(a._max.x, b._max.x),
			coMin(a._max.y, b._max.y),
			coMin(a._max.z, b._max.z))
	);
}

coFORCE_INLINE coBool32x4 coIsEmpty(const coAABox4& a)
{
	return a._min.x > a._max.x | a._min.y > a._max.y | a._min.z > a._max.z;
}

coFORCE_INLINE coAabb coMerge(const coAABox4& a)
{
	return coAabb(coVec3(coMin(a._min.x), coMin(a._min.y), coMin(a._min.z)),
		coVec3(coMax(a._max.x), coMax(a._max.y), coMax(a._max.z)));
}

coFORCE_INLINE void coSetElement(coAABox4& a, coUint elementIdx, const coAabb& b)
{
	a._min.x[elementIdx] = b.min.x;
	a._min.y[elementIdx] = b.min.y;
	a._min.z[elementIdx] = b.min.z;
	a._max.x[elementIdx] = b.min.x;
	a._max.y[elementIdx] = b.min.y;
	a._max.z[elementIdx] = b.min.z;
}