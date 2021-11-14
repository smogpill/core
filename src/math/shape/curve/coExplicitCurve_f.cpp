// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coExplicitCurve_f.h"
#include "math/scalar/coFloat_f.h"
#include "math/vector/coVec2_f.h"
#include "math/vector/coVec3_f.h"

template <class T>
coDynamicArray<T> _coBevel(const coDynamicArray<T>& curve, coFloat width, coUint nbSegments)
{
	if (curve.count < 3)
		return coDynamicArray<T>(curve);
	coDynamicArray<T> out;
	coReserve(out, curve.count + coMax(0, curve.count - 2));

	coPushBack(out, curve[0]);
	for (coUint idx = 1; idx < curve.count - 1; ++idx)
	{
		const T& prev = curve[idx - 1];
		const T& cur = curve[idx];
		const T& next = curve[idx + 1];
		const T prevDir = coNormalize(prev - cur);
		const T nextDir = coNormalize(next - cur);
		const coFloat dot = coDot(prevDir, nextDir).x;
		const coFloat angle = coAcos(dot);
		const coFloat offset = width / (2.0f * coSin(angle * 0.5f));
		coPushBack(out, cur + prevDir * offset);
		coPushBack(out, cur + nextDir * offset);
	}
	coPushBack(out, coBack(curve));
	return out;
}

coDynamicArray<coVec2> coBevel(const coDynamicArray<coVec2>& curve, coFloat width, coUint nbSegments)
{
	return _coBevel<coVec2>(curve, width, nbSegments);
}

coDynamicArray<coVec3> coBevel(const coDynamicArray<coVec3>& curve, coFloat width, coUint nbSegments)
{
	return _coBevel<coVec3>(curve, width, nbSegments);
}
