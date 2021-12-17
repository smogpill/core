// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coDistancePointTriangle_f.h"
#include <math/vector/coVec3_f.h>

// Based on Christer Ericson's book
coVec3 coClosestPtPointTriangle(const coVec3& p, const coVec3& a, const coVec3& b, const coVec3& c, coFloat& s, coFloat& t)
{
	// Check if P in vertex region outside A
	const coVec3 ab = b - a;
	const coVec3 ac = c - a;
	const coVec3 ap = p - a;
	const coFloat d1 = coDot(ab, ap).x;
	const coFloat d2 = coDot(ac, ap).x;
	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		s = 0.0f;
		t = 0.0f;
		return a;	// Barycentric coords 1,0,0
	}

	// Check if P in vertex region outside B
	const coVec3 bp = p - b;
	const coFloat d3 = coDot(ab, bp).x;
	const coFloat d4 = coDot(ac, bp).x;
	if (d3 >= 0.0f && d4 <= d3)
	{
		s = 1.0f;
		t = 0.0f;
		return b;	// Barycentric coords 0,1,0
	}

	// Check if P in edge region of AB, if so return projection of P onto AB
	const coFloat vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		const coFloat v = d1 / (d1 - d3);
		s = v;
		t = 0.0f;
		return a + v * ab;	// barycentric coords (1-v, v, 0)
	}

	// Check if P in vertex region outside C
	const coVec3 cp = p - c;
	const coFloat d5 = coDot(ab, cp).x;
	const coFloat d6 = coDot(ac, cp).x;
	if (d6 >= 0.0f && d5 <= d6)
	{
		s = 0.0f;
		t = 1.0f;
		return c;	// Barycentric coords 0,0,1
	}

	// Check if P in edge region of AC, if so return projection of P onto AC
	const coFloat vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		const coFloat w = d2 / (d2 - d6);
		s = 0.0f;
		t = w;
		return a + w * ac;	// barycentric coords (1-w, 0, w)
	}

	// Check if P in edge region of BC, if so return projection of P onto BC
	const coFloat va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		const coFloat w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		s = 1.0f - w;
		t = w;
		return b + w * (c - b);	// barycentric coords (0, 1-w, w)
	}

	// P inside face region. Compute Q through its barycentric coords (u,v,w)
	const coFloat denom = 1.0f / (va + vb + vc);
	const coFloat v = vb * denom;
	const coFloat w = vc * denom;
	s = v;
	t = w;
	return a + ab * v + ac * w;
}