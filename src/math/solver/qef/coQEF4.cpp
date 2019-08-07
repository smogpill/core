// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coQEF4.h"
#include "../../matrix/coSymMat4_f.h"
#include "../../matrix/coMat4_f.h"
#include "../../vector/coVec4_f.h"

// Implementations shared by:
// - https://github.com/nickgildea/qef (public domain)
// - https://github.com/Lin20/isosurface/tree/master/Isosurface/Isosurface/QEFSolver (MIT License)

const coFloat coQEF4::tolerance = 1e-6f;

coVec4 coSVD_VMulSym(const coMat4& a, const coVec4& v)
{
// 	return coVec3(coDot(a[0], v)
// 		, (a[0][1] * v.x) + (a[1][1] * v.y) + (a[1][2] * v.z)
// 		, (a[0][2] * v.x) + (a[1][2] * v.y) + (a[2][2] * v.z));
	//return a * v;
	return coVec4();
}

void coSVD_Rotate_XY(coFloat& x, coFloat& y, coFloat c, coFloat s)
{
	const coFloat u = x;
	const coFloat v = y;
	x = c * u - s * v;
	y = s * u + c * v;
}

void coSVD_RotateQ_XY(coFloat& x, coFloat& y, coFloat& a, coFloat c, coFloat s)
{
	const coFloat cc = c * c;
	const coFloat ss = s * s;
	const coFloat mx = 2.0f * c * s * a;
	const coFloat u = x;
	const coFloat v = y;
	x = cc * u - mx + ss * v;
	y = ss * u + mx + cc * v;
}

void coSVD_Rotate(coMat4& vtav, coMat4& v, coInt a, coInt  b)
{
	coASSERT(false);
	if (vtav[a][b] == 0.0f)
		return;

	coFloat c, s;
	coCalcSymmetricGivensCoefficients(vtav[a][a], vtav[a][b], vtav[b][b], c, s);
	coSVD_RotateQ_XY(vtav[a][a], vtav[b][b], vtav[a][b], c, s);
	coSVD_Rotate_XY(vtav[0][3 - b], vtav[1 - a][2], c, s);
	vtav[a][b] = 0.0f;

	coSVD_Rotate_XY(v[0][a], v[0][b], c, s);
	coSVD_Rotate_XY(v[1][a], v[1][b], c, s);
	coSVD_Rotate_XY(v[2][a], v[2][b], c, s);
}

void coRot01_post(coMat4& this_, coFloat c, coFloat s)
{
// 	const coFloat m00 = this_[0][0], m01 = this_[0][1], m10 = this_[1][0], m11 = this_[1][1], m20 = this_[2][0],
// 		m21 = this_[2][1];
// 	this_ = coMat3(
// 		coFloatx3(c * m00 - s * m01, s * m00 + c * m01, this_[0][2]),
// 		coFloatx3(c * m10 - s * m11, s * m10 + c * m11, this_[1][2]),
// 		coFloatx3(c * m20 - s * m21, s * m20 + c * m21, this_[2][2]));
}

void coRot02_post(coMat4& this_, coFloat c, coFloat s)
{
// 	const coFloat m00 = this_[0][0], m02 = this_[0][2], m10 = this_[1][0], m12 = this_[1][2],
// 		m20 = this_[2][0], m22 = this_[2][2];
// 	this_ = coMat3(
// 		coFloatx3(c * m00 - s * m02, this_[0][1], s * m00 + c * m02),
// 		coFloatx3(c * m10 - s * m12, this_[1][1], s * m10 + c * m12),
// 		coFloatx3(c * m20 - s * m22, this_[2][1], s * m20 + c * m22));
}

void coRot03_post(coMat4& this_, coFloat c, coFloat s)
{
}

void coRot12_post(coMat4& this_, coFloat c, coFloat s)
{
// 	const coFloat m01 = this_[0][1], m02 = this_[0][2], m11 = this_[1][1], m12 = this_[1][2],
// 		m21 = this_[2][1], m22 = this_[2][2];
// 	this_ = coMat3(
// 		coFloatx3(this_[0][0], c * m01 - s * m02, s * m01 + c * m02),
// 		coFloatx3(this_[1][0], c * m11 - s * m12, s * m11 + c * m12),
// 		coFloatx3(this_[2][0], c * m21 - s * m22, s * m21 + c * m22));
}

void coRot13_post(coMat4& this_, coFloat c, coFloat s)
{
}

void coRot23_post(coMat4& this_, coFloat c, coFloat s)
{
}

void coSVD_Rotate01(coSymMat4& vtav, coMat4& v)
{
	if (vtav.m01 == 0.0f)
		return;
 
 	coFloat c = 0.0f;
 	coFloat s = 0.0f;
 	coRot01(vtav, c, s);
 	c = 0.0f;
 	s = 0.0f;
 	coRot01_post(v, c, s);
}

void coSVD_Rotate02(coSymMat4& vtav, coMat4& v)
{
	if (vtav.m02 == 0.0f)
		return;

	coFloat c = 0.0f;
	coFloat s = 0.0f;
	coRot02(vtav, c, s);
	c = 0.0f;
	s = 0.0f;
	coRot02_post(v, c, s);
}

void coSVD_Rotate03(coSymMat4& vtav, coMat4& v)
{
	if (vtav.m03 == 0.0f)
		return;

	coFloat c = 0.0f;
	coFloat s = 0.0f;
	coRot03(vtav, c, s);
	c = 0.0f;
	s = 0.0f;
	coRot03_post(v, c, s);
}

void coSVD_Rotate12(coSymMat4& vtav, coMat4& v)
{
	if (vtav.m12 == 0)
		return;

	coFloat c = 0.0f;
	coFloat s = 0.0f;
	coRot12(vtav, c, s);
	c = 0.0f;
	s = 0.0f;
	coRot12_post(v, c, s);
}

void coSVD_Rotate13(coSymMat4& vtav, coMat4& v)
{
	if (vtav.m13 == 0)
		return;

	coFloat c = 0.0f;
	coFloat s = 0.0f;
	coRot13(vtav, c, s);
	c = 0.0f;
	s = 0.0f;
	coRot13_post(v, c, s);
}

void coSVD_Rotate23(coSymMat4& vtav, coMat4& v)
{
	if (vtav.m23 == 0)
		return;

	coFloat c = 0.0f;
	coFloat s = 0.0f;
	coRot23(vtav, c, s);
	c = 0.0f;
	s = 0.0f;
	coRot23_post(v, c, s);
}

void coSVD_SolveSym(coVec4& sigma, coMat4& v, const coMat4& a)
{
 	coSymMat4 vtav(a);
 
 	const coFloat delta = coQEF4::tolerance * coFrobeniusNorm(vtav);
 	for (coInt i = 0; i < 10 && coOff(vtav) > delta; ++i)
 	{
 		coSVD_Rotate01(vtav, v);
		coSVD_Rotate02(vtav, v);
		coSVD_Rotate03(vtav, v);
		coSVD_Rotate12(vtav, v);
		coSVD_Rotate13(vtav, v);
		coSVD_Rotate23(vtav, v);
 	}
 	sigma = coVec4(vtav.m00, vtav.m11, vtav.m22, vtav.m33);
}

void coSVD_ComputePseudoInverse(coMat4& out, const coVec4& sigma, const coMat4& v)
{
	coVec4 invSigma;
	invSigma.x = (coAbs(sigma.x) < coQEF4::tolerance || coAbs(1.0f / sigma.x) < coQEF4::tolerance) ? 0.0f : (1.0f / sigma.x);
	invSigma.y = (coAbs(sigma.y) < coQEF4::tolerance || coAbs(1.0f / sigma.y) < coQEF4::tolerance) ? 0.0f : (1.0f / sigma.y);
	invSigma.z = (coAbs(sigma.z) < coQEF4::tolerance || coAbs(1.0f / sigma.z) < coQEF4::tolerance) ? 0.0f : (1.0f / sigma.z);
	invSigma.w = (coAbs(sigma.w) < coQEF4::tolerance || coAbs(1.0f / sigma.w) < coQEF4::tolerance) ? 0.0f : (1.0f / sigma.w);
	out = v * coMat4(invSigma) * coTranspose(v);
}

void coSVD_Solve_ATA_ATb(coVec4& out, const coMat4& ATA, const coVec4& ATb)
{
	coMat4 V;
	coVec4 sigma(nullptr);
	coSVD_SolveSym(sigma, V, ATA);
	coMat4 invV(nullptr);
	coSVD_ComputePseudoInverse(invV, sigma, V);
	out = invV * ATb;
}

coFloatx4 coQEF_ComputeError(const coMat4& A, const coVec4& x, const coVec4& b)
{
	const coVec4 vTmp = b - coSVD_VMulSym(A, x);
	return coSquareLength(vTmp);
}

coFloat coQEF4::Solve(coVec4& posOut)
{
	coASSERT(nb);
	massPoint = pointAccum / coFloat(nb);
	const coVec4 ATb2 = ATb - coSVD_VMulSym(ATA, massPoint);
	coSVD_Solve_ATA_ATb(posOut, ATA, ATb2);
	const coFloatx4 error = coQEF_ComputeError(ATA, posOut, ATb2);
	posOut += massPoint;

	const coVec4 atax = coSVD_VMulSym(ATA, posOut);
	return (coDot(posOut, atax) - 2.0f * coDot(posOut, ATb) + btb).x;
	//return error.x;
}

void coQEF4::Add(const coVec4& A, const coVec4& b)
{
	const coVec4 Ax = coBroadcastX(A);
	const coVec4 Ay = coBroadcastY(A);
	const coVec4 Az = coBroadcastZ(A);
	const coVec4 Aw = coBroadcastW(A);
	ATA.c0 += Ax * A;
	ATA.c1 += Ay * A;
	ATA.c2 += Az * A;
	ATA.c3 += Aw * A;
	ATb += A * b;
	btb += (b * b).x;
	//pointAccum += pos;
	++nb;
}

void coQEF4::Add(const coQEF4& qef)
{
	ATA += qef.ATA;
	ATb += qef.ATb;
	btb += qef.btb;
	pointAccum += qef.pointAccum;
	nb += qef.nb;
}
