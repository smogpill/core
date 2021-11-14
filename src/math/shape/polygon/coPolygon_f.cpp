// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coPolygon_f.h"
#include "math/scalar/coFloat_f.h"
#include "math/vector/coVec2_f.h"
#include "math/vector/coVec3_f.h"

void coMakeCircle(coDynamicArray<coVec2>& out, const coFloat radius, coUint nbSegments)
{
	coClear(out);
	coResize(out, nbSegments);
	for (coUint i = 0; i < nbSegments; ++i)
	{
		const coFloat u = coFloat(i) / nbSegments;
		const coFloat angle = u * coFloat_2Pi;
		coVec2 p(nullptr);
		p.x = coCos(angle) * radius;
		p.y = coSin(angle) * radius;
		out[i] = p;
	}
}
