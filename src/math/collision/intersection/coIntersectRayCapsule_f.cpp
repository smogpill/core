// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coIntersectRayCapsule_f.h"
#include "coIntersectRaySphere_f.h"

// PT: ray-capsule intersection code, originally from the old Magic Software library.
coUint32 _coIntersectRayCapsuleInternal(const coVec3& origin, const coVec3& dir, const coVec3& p0, const coVec3& p1, coFloat radius, coFloat s[2])
{
	//=====
	// Impl from Gu::intersectRayCapsuleInternal(), \physx\source\geomutils\src\intersection\GuIntersectionRayCapsule.cpp
	// Most comments are from the original code.
	// From the open source version of the PhysX SDK
	//=====

	// set up quadratic Q(t) = a*t^2 + 2*b*t + c
	coVec3 kW = p1 - p0;
	const coFloat fWLength = coLength(kW).x;
	if (fWLength != 0.0f)
		kW /= fWLength;

	// PT: if the capsule is in fact a sphere, switch back to dedicated sphere code.
	// This is not just an optimization, the rest of the code fails otherwise.
	if (fWLength <= 1e-6f)
	{
		const coFloat d0 = coSquareLength(origin - p0).x;
		const coFloat d1 = coSquareLength(origin - p1).x;
		const coFloat approxLength = (coMax(d0, d1) + radius) * 2.0f;
		return coUint32(coIntersectRaySphere(origin, dir, approxLength, p0, radius, s[0]));
	}

	// generate orthonormal basis
	coVec3 kU(0.0f);

	if (fWLength > 0.0f)
	{
		coFloat fInvLength;
		if (coAbs(kW.x) >= coAbs(kW.y))
		{
			// W.x or W.z is the largest magnitude component, swap them
			fInvLength = coInvSquareRoot(kW.x * kW.x + kW.z * kW.z);
			kU.x = -kW.z * fInvLength;
			kU.y = 0.0f;
			kU.z = kW.x * fInvLength;
		}
		else
		{
			// W.y or W.z is the largest magnitude component, swap them
			fInvLength = coInvSquareRoot(kW.y * kW.y + kW.z * kW.z);
			kU.x = 0.0f;
			kU.y = kW.z * fInvLength;
			kU.z = -kW.y * fInvLength;
		}
	}

	coVec3 kV = coCross(kW, kU);
	kV = coNormalize(kV);	// PT: fixed november, 24, 2004. This is a bug in Magic.

	// compute intersection

	coVec3 kD(coDot(kU, dir), coDot(kV, dir), coDot(kW, dir));
	const coFloat fDLength = coLength(kD).x;
	const coFloat fInvDLength = fDLength != 0.0f ? 1.0f / fDLength : 0.0f;
	kD *= fInvDLength;

	const coVec3 kDiff = origin - p0;
	const coVec3 kP(coDot(kU, kDiff), coDot(kV, kDiff), coDot(kW, kDiff));
	const coFloat fRadiusSqr = radius * radius;

	// Is the velocity parallel to the capsule direction? (or zero)
	if (coAbs(kD.z) >= 1.0f - FLT_EPSILON || fDLength < FLT_EPSILON)
	{
		const coFloat fAxisDir = coDot(dir, kW).x;

		const coFloat fDiscr = fRadiusSqr - kP.x * kP.x - kP.y * kP.y;
		if (fAxisDir < 0 && fDiscr >= 0.0f)
		{
			// Velocity anti-parallel to the capsule direction
			const coFloat fRoot = coSquareRoot(fDiscr);
			s[0] = (kP.z + fRoot) * fInvDLength;
			s[1] = -(fWLength - kP.z + fRoot) * fInvDLength;
			return 2;
		}
		else if (fAxisDir > 0 && fDiscr >= 0.0f)
		{
			// Velocity parallel to the capsule direction
			const coFloat fRoot = coSquareRoot(fDiscr);
			s[0] = -(kP.z + fRoot) * fInvDLength;
			s[1] = (fWLength - kP.z + fRoot) * fInvDLength;
			return 2;
		}
		else
		{
			// sphere heading wrong direction, or no velocity at all
			return 0;
		}
	}

	// test intersection with infinite cylinder
	coFloat fA = kD.x * kD.x + kD.y * kD.y;
	coFloat fB = kP.x * kD.x + kP.y * kD.y;
	coFloat fC = kP.x * kP.x + kP.y * kP.y - fRadiusSqr;
	coFloat fDiscr = fB * fB - fA * fC;
	if (fDiscr < 0.0f)
	{
		// line does not intersect infinite cylinder
		return 0;
	}

	coUint32 iQuantity = 0;

	if (fDiscr > 0.0f)
	{
		// line intersects infinite cylinder in two places
		const coFloat fRoot = coSquareRoot(fDiscr);
		const coFloat fInv = 1.0f / fA;
		coFloat fT = (-fB - fRoot) * fInv;
		coFloat fTmp = kP.z + fT * kD.z;
		const float epsilon = 1e-3f;	// PT: see TA35174
		if (fTmp >= -epsilon && fTmp <= fWLength + epsilon)
			s[iQuantity++] = fT * fInvDLength;

		fT = (-fB + fRoot) * fInv;
		fTmp = kP.z + fT * kD.z;
		if (fTmp >= -epsilon && fTmp <= fWLength + epsilon)
			s[iQuantity++] = fT * fInvDLength;

		if (iQuantity == 2)
		{
			// line intersects capsule wall in two places
			return 2;
		}
	}
	else
	{
		// line is tangent to infinite cylinder
		const coFloat fT = -fB / fA;
		const coFloat fTmp = kP.z + fT * kD.z;
		if (0.0f <= fTmp && fTmp <= fWLength)
		{
			s[0] = fT * fInvDLength;
			return 1;
		}
	}

	// test intersection with bottom hemisphere
	// fA = 1
	fB += kP.z * kD.z;
	fC += kP.z * kP.z;
	fDiscr = fB * fB - fC;
	if (fDiscr > 0.0f)
	{
		const coFloat fRoot = coSquareRoot(fDiscr);
		coFloat fT = -fB - fRoot;
		coFloat fTmp = kP.z + fT * kD.z;
		if (fTmp <= 0.0f)
		{
			s[iQuantity++] = fT * fInvDLength;
			if (iQuantity == 2)
				return 2;
		}

		fT = -fB + fRoot;
		fTmp = kP.z + fT * kD.z;
		if (fTmp <= 0.0f)
		{
			s[iQuantity++] = fT * fInvDLength;
			if (iQuantity == 2)
				return 2;
		}
	}
	else if (fDiscr == 0.0f)
	{
		const coFloat fT = -fB;
		const coFloat fTmp = kP.z + fT * kD.z;
		if (fTmp <= 0.0f)
		{
			s[iQuantity++] = fT * fInvDLength;
			if (iQuantity == 2)
				return 2;
		}
	}

	// test intersection with top hemisphere
	// fA = 1
	fB -= kD.z * fWLength;
	fC += fWLength * (fWLength - 2.0f * kP.z);

	fDiscr = fB * fB - fC;
	if (fDiscr > 0.0f)
	{
		const coFloat fRoot = coSquareRoot(fDiscr);
		coFloat fT = -fB - fRoot;
		coFloat fTmp = kP.z + fT * kD.z;
		if (fTmp >= fWLength)
		{
			s[iQuantity++] = fT * fInvDLength;
			if (iQuantity == 2)
				return 2;
		}

		fT = -fB + fRoot;
		fTmp = kP.z + fT * kD.z;
		if (fTmp >= fWLength)
		{
			s[iQuantity++] = fT * fInvDLength;
			if (iQuantity == 2)
				return 2;
		}
	}
	else if (fDiscr == 0.0f)
	{
		const coFloat fT = -fB;
		const coFloat fTmp = kP.z + fT * kD.z;
		if (fTmp >= fWLength)
		{
			s[iQuantity++] = fT * fInvDLength;
			if (iQuantity == 2)
				return 2;
		}
	}
	return iQuantity;
}
