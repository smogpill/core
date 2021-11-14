// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coExplicitCurve_f.h"
#include "math/scalar/coFloat_f.h"
#include "math/vector/coVec3_f.h"

coDynamicArray<coVec3> coBevel(const coDynamicArray<coVec3>& curve, coFloat width, coUint nbSegments)
{
	if (curve.count < 3)
		return coDynamicArray<coVec3>(curve);
	coDynamicArray<coVec3> out;
	coReserve(out, curve.count + coMax(0, curve.count - 2));

	coPushBack(out, curve[0]);
	for (coUint idx = 1; idx < curve.count - 1; ++idx)
	{
		const coVec3& prev = curve[idx - 1];
		const coVec3& cur = curve[idx];
		const coVec3& next = curve[idx + 1];
		const coVec3 prevDir = coNormalize(prev - cur);
		const coVec3 nextDir = coNormalize(next - cur);
		const coFloat dot = coDot(prevDir, nextDir).x;
		const coFloat angle = coAcos(dot);
		const coFloat offset = width / (2.0f * coSin(angle * 0.5f));
		coPushBack(out, cur + prevDir * offset);
		coPushBack(out, cur + nextDir * offset);
	}
	coPushBack(out, coBack(curve));
	return out;
}
