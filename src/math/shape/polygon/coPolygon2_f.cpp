// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coPolygon2_f.h"
#include "math/scalar/coFloat_f.h"
#include "math/vector/coVec2_f.h"
#include "math/vector/coVec3_f.h"
#include "math/shape/curve/coExplicitCurve_f.h"

void coMakeCircle(coPolygon2& out, const coFloat radius, coUint nbSegments)
{
	coClear(out.vertices);
	coResize(out.vertices, nbSegments);
	for (coUint i = 0; i < nbSegments; ++i)
	{
		const coFloat u = coFloat(i) / nbSegments;
		const coFloat angle = u * coFloat_2Pi;
		coVec2 p(nullptr);
		p.x = coCos(angle) * radius;
		p.y = coSin(angle) * radius;
		out.vertices[i] = p;
	}
}

void coMakeRectangle(coPolygon2& out, const coVec2& size)
{
	coClear(out.vertices);
	coResize(out.vertices, 4);
	const coVec2 halfSize = size * 0.5f;
	out.vertices[0] = coVec2(-halfSize.x, -halfSize.y);
	out.vertices[1] = coVec2(halfSize.x, -halfSize.y);
	out.vertices[2] = coVec2(halfSize.x, halfSize.y);
	out.vertices[3] = coVec2(-halfSize.x, halfSize.y);
}

void coBevel(coPolygon2& out, coFloat width, coUint nbSegments)
{
	out.vertices = coBevel(out.vertices, width, nbSegments, true);
}
