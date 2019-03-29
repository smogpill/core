// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "math/solver/qef/coQEF.h"
#include "math/vector/coVec3_f.h"
#include "math/vector/coVec4.h"
#include "math/matrix/coMat3_f.h"

coVec3 coSVD_VMulSym(const coMat3& a, const coVec3& v)
{
	return coVec3(coDot(a[0], v)
		, (a[0][1] * v.x) + (a[1][1] * v.y) + (a[1][2] * v.z)
		, (a[0][2] * v.x) + (a[1][2] * v.y) + (a[2][2] * v.z));
}

void coComputeGivensCoeffsSym(coFloat& c, coFloat& s, coFloat a_pp, coFloat a_pq, coFloat a_qq)
{
	if (a_pq == 0.0f)
	{
		c = 1.0f;
		s = 0.0f;
		return;
	}
	const coFloat tau = (a_qq - a_pp) / (2.0f * a_pq);
	const coFloat stt = coSquareRoot(1.0f + tau * tau);
	const coFloat tan = 1.0f / ((tau >= 0.0f) ? (tau + stt) : (tau - stt));
	c = coInvSquareRoot(1.0f + tan * tan);
	s = tan * c;
}

void coSVD_Rotate_xy(coFloat& x, coFloat& y, coFloat c, coFloat s)
{
	const coFloat u = x;
	const coFloat v = y;
	x = c * u - s * v;
	y = s * u + c * v;
}

void coSVD_RotateQ_xy(coFloat& x, coFloat& y, coFloat a, coFloat c, coFloat s)
{
	const coFloat cc = c * c;
	const coFloat ss = s * s;
	const coFloat mx = 2.0f * c * s * a;
	const coFloat u = x;
	const coFloat v = y;
	x = cc * u - mx + ss * v;
	y = ss * u + mx + cc * v;
}

void coSVD_Rotate(coMat3& vtav, coMat3& v, coInt a, coInt b)
{
	if (vtav[a][b] == 0.0f)
		return;

	coFloat c, s;
	coComputeGivensCoeffsSym(c, s, vtav[a][a], vtav[a][b], vtav[b][b]);
	coSVD_RotateQ_xy(vtav[a][a], vtav[b][b], vtav[a][b], c, s);
	coSVD_Rotate_xy(vtav[0][3 - b], vtav[1 - a][2], c, s);
	vtav[a][b] = 0.0f;

	coSVD_Rotate_xy(v[0][a], v[0][b], c, s);
	coSVD_Rotate_xy(v[1][a], v[1][b], c, s);
	coSVD_Rotate_xy(v[2][a], v[2][b], c, s);
}

void coSVD_SolveSym(coVec3& sigma, coMat3& v, const coMat3& a)
{
	// assuming that A is symmetric: can optimize all operations for 
	// the upper right triagonal
	coMat3 vtav = a;
	// assuming V is identity: you can also pass a matrix the rotations
	// should be applied to
	// U is not computed
	for (coInt i = 0; i < 5; ++i)
	{
		coSVD_Rotate(vtav, v, 0, 1);
		coSVD_Rotate(vtav, v, 0, 2);
		coSVD_Rotate(vtav, v, 1, 2);
	}
	sigma = coVec3(vtav[0][0], vtav[1][1], vtav[2][2]);
}

coFloatx4 coSVD_ComputeInvDet(coFloatx4 x, coFloatx4 tolerance)
{
	return (coAbs(x) < tolerance || coAbs(1.0f / x) < tolerance) ? 0.0f : (1.0f / x);
}

void coSVD_ComputePseudoInverse(coMat3& out, const coVec3& sigma, const coMat3& v)
{
	const coFloatx4 d0 = coSVD_ComputeInvDet(sigma[0], 1e-6f);
	const coFloatx4 d1 = coSVD_ComputeInvDet(sigma[1], 1e-6f);
	const coFloatx4 d2 = coSVD_ComputeInvDet(sigma[2], 1e-6f);
	out.c0 = coVec3(
		v[0][0] * d0 * v[0][0] + v[0][1] * d1 * v[0][1] + v[0][2] * d2 * v[0][2],
		v[0][0] * d0 * v[1][0] + v[0][1] * d1 * v[1][1] + v[0][2] * d2 * v[1][2],
		v[0][0] * d0 * v[2][0] + v[0][1] * d1 * v[2][1] + v[0][2] * d2 * v[2][2]);
	out.c1 = coVec3(
		v[1][0] * d0 * v[0][0] + v[1][1] * d1 * v[0][1] + v[1][2] * d2 * v[0][2],
		v[1][0] * d0 * v[1][0] + v[1][1] * d1 * v[1][1] + v[1][2] * d2 * v[1][2],
		v[1][0] * d0 * v[2][0] + v[1][1] * d1 * v[2][1] + v[1][2] * d2 * v[2][2]);
	out.c2 = coVec3(
		v[2][0] * d0 * v[0][0] + v[2][1] * d1 * v[0][1] + v[2][2] * d2 * v[0][2],
		v[2][0] * d0 * v[1][0] + v[2][1] * d1 * v[1][1] + v[2][2] * d2 * v[1][2],
		v[2][0] * d0 * v[2][0] + v[2][1] * d1 * v[2][1] + v[2][2] * d2 * v[2][2]);
}

void coSVD_Solve_ATA_ATb(coVec3& out, const coMat3& ATA, const coVec3& ATb)
{
	coMat3 V;
	coVec3 sigma(nullptr);
	coSVD_SolveSym(sigma, V, ATA);
	coMat3 invV(nullptr);
	coSVD_ComputePseudoInverse(invV, sigma, V);
	out = invV * ATb;
}

coFloatx4 coQEF_ComputeError(const coMat3& A, const coVec3& x, const coVec3& b)
{
	const coVec3 vTmp = b - coSVD_VMulSym(A, x);
	return coSquareLength(vTmp);
}

void coQEF::Add(const coVec3& pos, const coVec3& normal)
{
	ATA[0][0] += normal.x * normal.x;
	ATA[0][1] += normal.x * normal.y;
	ATA[0][2] += normal.x * normal.z;
	ATA[1][1] += normal.y * normal.y;
	ATA[1][2] += normal.y * normal.z;
	ATA[2][2] += normal.z * normal.z;

	const coFloatx4 b = coDot(pos, normal);
	ATb += normal * b;
	pointAccum += coVec4(pos, 1.0f);
}

void coQEF::Add(const coQEF& qef)
{
	ATA[0][0] += qef.ATA[0][0];
	ATA[0][1] += qef.ATA[0][1];
	ATA[0][2] += qef.ATA[0][2];
	ATA[1][1] += qef.ATA[1][1];
	ATA[1][2] += qef.ATA[1][2];
	ATA[2][2] += qef.ATA[2][2];
	ATb += qef.ATb;
	pointAccum += qef.pointAccum;
}

coFloat coQEF::Solve(coVec3& posOut)
{
	coASSERT(pointAccum.w != 0.0f);
	massPoint = coVec3(pointAccum / pointAccum.w);
	ATb -= coSVD_VMulSym(ATA, massPoint);
	coSVD_Solve_ATA_ATb(posOut, ATA, ATb);
	const coFloatx4 error = coQEF_ComputeError(ATA, posOut, ATb);
	posOut += massPoint;
	return error.x;
}
