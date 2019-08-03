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
	return coVec4();
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

void coRot12_post(coMat4& this_, coFloat c, coFloat s)
{
// 	const coFloat m01 = this_[0][1], m02 = this_[0][2], m11 = this_[1][1], m12 = this_[1][2],
// 		m21 = this_[2][1], m22 = this_[2][2];
// 	this_ = coMat3(
// 		coFloatx3(this_[0][0], c * m01 - s * m02, s * m01 + c * m02),
// 		coFloatx3(this_[1][0], c * m11 - s * m12, s * m11 + c * m12),
// 		coFloatx3(this_[2][0], c * m21 - s * m22, s * m21 + c * m22));
}

void coSVD_Rotate01(coSymMat4& vtav, coMat4& v)
{
// 	if (vtav.m01 == 0.0f)
// 		return;
// 
// 	coFloat c = 0.0f;
// 	coFloat s = 0.0f;
// 	coRot01(vtav, c, s);
// 	c = 0.0f;
// 	s = 0.0f;
// 	coRot01_post(v, c, s);
}

void coSVD_Rotate02(coSymMat4& vtav, coMat4& v)
{
// 	if (vtav.m02 == 0.0f)
// 		return;
// 
// 	coFloat c = 0.0f;
// 	coFloat s = 0.0f;
// 	coRot02(vtav, c, s);
// 	c = 0.0f;
// 	s = 0.0f;
// 	coRot02_post(v, c, s);
}

void coSVD_Rotate12(coSymMat4& vtav, coMat4& v)
{
// 	if (vtav.m12 == 0)
// 		return;
// 
// 	coFloat c = 0.0f;
// 	coFloat s = 0.0f;
// 	coRot12(vtav, c, s);
// 	c = 0.0f;
// 	s = 0.0f;
// 	coRot12_post(v, c, s);
}

void coSVD_SolveSym(coVec4& sigma, coMat4& v, const coMat4& a)
{
	// assuming that A is symmetric: can optimize all operations for 
	// the upper right triagonal
// 	coSymMat4 vtav(a);
// 
// 	const coFloat delta = coQEF4::tolerance * coFNorm(vtav);
// 	// assuming V is identity: you can also pass a matrix the rotations
// 	// should be applied to
// 	// U is not computed
// 	for (coInt i = 0; i < 5 && coOff(vtav) > delta; ++i)
// 	{
// 		coSVD_Rotate01(vtav, v);
// 		coSVD_Rotate02(vtav, v);
// 		coSVD_Rotate12(vtav, v);
// 	}
// 	sigma = coVec4(vtav.m00, vtav.m11, vtav.m22, vtav.m33);
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

void coQEF4::Add(const coVec4& pos, const coVec4& normal)
{
	ATA[0][0] += normal.x * normal.x;
	ATA[0][1] += normal.x * normal.y;
	ATA[0][2] += normal.x * normal.z;
	ATA[0][3] += normal.x * normal.w;
	ATA[1][1] += normal.y * normal.y;
	ATA[1][2] += normal.y * normal.z;
	ATA[1][3] += normal.y * normal.w;
	ATA[2][2] += normal.z * normal.z;
	ATA[2][3] += normal.z * normal.w;
	ATA[3][3] += normal.w * normal.w;

	const coFloatx4 b = coDot(pos, normal);
	ATb += normal * b;
	btb += (b * b).x;
	pointAccum += pos;
	++nbPoints;
}

void coQEF4::Add(const coQEF4& qef)
{
	ATA += qef.ATA;
	ATb += qef.ATb;
	btb += qef.btb;
	pointAccum += qef.pointAccum;
	nbPoints += qef.nbPoints;
}
