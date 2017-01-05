// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "math/shape/coSegment_f.h"
#include "math/shape/coSegment.h"
#include "math/vector/coFloatx3_f.h"

coBool coIntersectXY(const coSegment& _a, const coSegment& _b, coFloatx3& _hit)
{
	// http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
	// http://ideone.com/PnPJgb

	const coFloatx3 CmP(_b.p0.x - _a.p0.x, _b.p0.y - _a.p0.y, 0.0f);
	const coFloatx3 r(_a.p1.x - _a.p0.x, _a.p1.y - _a.p0.y, 0.0f);
	const coFloatx3 s(_b.p1.x - _b.p0.x, _b.p1.y - _b.p0.y, 0.0f);

	const coFloat CmPxr = CmP.x * r.y - CmP.y * r.x;
	const coFloat CmPxs = CmP.x * s.y - CmP.y * s.x;
	const coFloat rxs = r.x * s.y - r.y * s.x;

	// Collinear?
	if (CmPxr == 0.0f)
		return false;

	// Parallel?
	if (rxs == 0.0f)
		return false;

	const coFloat rxsr = 1.0f / rxs;
	const coFloat t = CmPxs * rxsr;
	const coFloat u = CmPxr * rxsr;

	const coBool intersect = (t >= 0.0f) && (t <= 1.0f) && (u >= 0.0f) && (u <= 1.0f);
	if (intersect)
		_hit = _a.p0 + t * r;
	return intersect;
}
