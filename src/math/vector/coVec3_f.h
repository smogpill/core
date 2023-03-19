// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/coCppExtensions.h"
#include "math/scalar/coFloat_f.h"
#include "math/vector/coVec3.h"
#include "math/vector/coBool32x3_f.h"
#include "math/vector/coFloatx3_f.h"
#include "io/stream/coBinaryInputStream.h"
#include "io/stream/coBinaryOutputStream.h"
#include "debug/log/coAssert.h"

coFORCE_INLINE coFloatx4 coDot(const coVec3& _a, const coVec3& _b)
{
	const coFloatx4 mul = coBitCast<coFloatx4>(_a) * coBitCast<coFloatx4>(_b);
	return coBroadcastX(mul) + coBroadcastY(mul) + coBroadcastZ(mul);
}
coFORCE_INLINE coFloatx3 coSum(const coVec3& a)
{
	return coBroadcastX(a) + coBroadcastY(a) + coBroadcastZ(a);
}
coFORCE_INLINE coVec3 coCross(const coVec3& _a, const coVec3& _b)
{
	const coVec3 tmp0 = coShuffle<1, 2, 0>(_a, _a);
	const coVec3 tmp1 = coShuffle<2, 0, 1>(_b, _b);
	const coVec3 tmp2 = coShuffle<2, 0, 1>(_a, _a);
	const coVec3 tmp3 = coShuffle<1, 2, 0>(_b, _b);
	return tmp0 * tmp1 - tmp2 * tmp3;
}
coFORCE_INLINE coVec3 coXY(const coVec3& a) { return coVec3(a.x, a.y, 0.0f); }
coFORCE_INLINE coFloatx4 coSquareLength(const coVec3& _a) { return coDot(_a, _a); }
coFORCE_INLINE coFloatx4 coLength(const coVec3& _a) { return coSquareRoot(coDot(_a, _a)); }
coFORCE_INLINE coVec3 coNormalize(const coVec3& _a, const coVec3& epsilon = coVec3(1e-10f)) { return coNearEqual0(_a, epsilon) ? coVec3(0.0f) : _a * coInvSquareRoot(coDot(_a, _a)); }
coFORCE_INLINE coBool32x3 coIsNormalized(const coVec3& _a, const coVec3& _squareEpsilon = coVec3(1e-3f)) { return coNearEqual(coSquareLength(_a), coVec3(1.0f), _squareEpsilon); }
/// coDot(coNormalize(a), coNormalize(b))
coFORCE_INLINE coFloatx4 coNormalizeDot(const coVec3& a, const coVec3& b) { return coDot(a, b) * coInvSquareRoot(coSquareLength(a)*coSquareLength(b)); }
coFORCE_INLINE coVec3 coAnyOrthogonal(const coVec3& _a)
{
	const coFloatx3 absA = coAbs(_a);
	switch (coMinIndex(absA.x, absA.y, absA.z))
	{
	case 0: return coCross(_a, coVec3(1.0f, 0.0f, 0.0f));
	case 1: return coCross(_a, coVec3(0.0f, 1.0f, 0.0f));
	case 2: return coCross(_a, coVec3(0.0f, 0.0f, 1.0f));
	default: coASSERT(false); return coVec3();
	}
}

coFORCE_INLINE coVec3 coReflect(const coVec3& incomingVec, const coVec3& normal)
{
	return incomingVec - normal * 2.0f * coDot(incomingVec, normal);
}

coFORCE_INLINE coVec3 coLambertNoTangent(const coVec3& nor, coFloat u, coFloat v)
{
	// https://www.shadertoy.com/view/MsdGzl
	// method 3 by fizzer: http://www.amietia.com/lambertnotangent.html
	const coFloat a = 6.2831853f * v;
	u = 2.0f * u - 1.0f;
	const coFloat f = coSquareRoot(1.0f - u * u);
	return coNormalize(nor + coVec3(f * coCos(a), f * coSin(a), u));
}

coFORCE_INLINE void coVec3::Write(coBinaryOutputStream& stream) const
{
	stream << x;
	stream << y;
	stream << z;
}

coFORCE_INLINE void coVec3::Read(coBinaryInputStream& stream)
{
	stream >> x;
	stream >> y;
	stream >> z;
}