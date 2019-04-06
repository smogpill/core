// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "math/solver/qef/coQEF.h"
#include "math/vector/coVec3_f.h"
#include "math/vector/coVec4.h"
#include "math/matrix/coMat3_f.h"

// Implementations shared by:
// - https://github.com/nickgildea/qef (public domain)
// - https://github.com/Lin20/isosurface/tree/master/Isosurface/Isosurface/QEFSolver (MIT License)

class SMat3
{
public:
	float m00, m01, m02, m11, m12, m22;

	SMat3()
	{
		Clear();
	}

	SMat3(float m00, float m01, float m02,
		float m11, float m12, float m22)
	{
		SetSymmetric(m00, m01, m02, m11, m12, m22);
	}

	SMat3(const coMat3& m)
	{
		m00 = m[0][0];
		m01 = m[0][1];
		m02 = m[0][2];
		m11 = m[1][1];
		m12 = m[1][2];
		m22 = m[2][2];
	}

	void Clear()
	{
		SetSymmetric(0, 0, 0, 0, 0, 0);
	}

	void SetSymmetric(float a00, float a01, float a02, float a11, float a12, float a22)
	{
		this->m00 = a00;
		this->m01 = a01;
		this->m02 = a02;
		this->m11 = a11;
		this->m12 = a12;
		this->m22 = a22;
	}

	void SetSymmetric(const SMat3& rhs)
	{
		SetSymmetric(rhs.m00, rhs.m01, rhs.m02, rhs.m11, rhs.m12, rhs.m22);
	}

	float Fnorm() const
	{
		return (float)coSquareRoot((m00 * m00) + (m01 * m01) + (m02 * m02)
			+ (m01 * m01) + (m11 * m11) + (m12 * m12)
			+ (m02 * m02) + (m12 * m12) + (m22 * m22));
	}

	float Off() const
	{
		return (float)coSquareRoot(2 * ((m01 * m01) + (m02 * m02) + (m12 * m12)));
	}

	SMat3 Mul_ata(const coMat3& a) const
	{
		SMat3 m;
		m.SetSymmetric(
			a[0][0] * a[0][0] + a[1][0] * a[1][0] + a[2][0] * a[2][0],
			a[0][0] * a[0][1] + a[1][0] * a[1][1] + a[2][0] * a[2][1],
			a[0][0] * a[0][2] + a[1][0] * a[1][2] + a[2][0] * a[2][2],
			a[0][1] * a[0][1] + a[1][1] * a[1][1] + a[2][1] * a[2][1],
			a[0][1] * a[0][2] + a[1][1] * a[1][2] + a[2][1] * a[2][2],
			a[0][2] * a[0][2] + a[1][2] * a[1][2] + a[2][2] * a[2][2]);
		return m;
	}

	coVec3 Vmul(const coVec3& v)
	{
		coVec3 o;
		o.x = (m00 * v.x) + (m01 * v.y) + (m02 * v.z);
		o.y = (m01 * v.x) + (m11 * v.y) + (m12 * v.z);
		o.z = (m02 * v.x) + (m12 * v.y) + (m22 * v.z);
		return o;
	}

	static void CalcSymmetricGivensCoefficients(float a_pp, float a_pq, float a_qq, float& c, float& s)
	{
		if (a_pq == 0)
		{
			c = 1;
			s = 0;
			return;
		}

		float tau = (a_qq - a_pp) / (2.0f * a_pq);
		float stt = (float)coSquareRoot(1.0f + tau * tau);
		float tan = 1.0f / ((tau >= 0) ? (tau + stt) : (tau - stt));
		c = 1.0f / (float)coSquareRoot(1.0f + tan * tan);
		s = tan * c;
	}

	void Rot01(float& c, float& s)
	{
		CalcSymmetricGivensCoefficients(m00, m01, m11, c, s);
		float cc = c * c;
		float ss = s * s;
		float mix = 2 * c * s * m01;
		SetSymmetric(cc * m00 - mix + ss * m11, 0, c * m02 - s * m12,
			ss * m00 + mix + cc * m11, s * m02 + c * m12, m22);
	}

	void Rot02(float& c, float& s)
	{
		CalcSymmetricGivensCoefficients(m00, m02, m22, c, s);
		float cc = c * c;
		float ss = s * s;
		float mix = 2 * c * s * m02;
		SetSymmetric(cc * m00 - mix + ss * m22, c * m01 - s * m12, 0,
			m11, s * m01 + c * m12, ss * m00 + mix + cc * m22);
	}

	void Rot12(float& c, float& s)
	{
		CalcSymmetricGivensCoefficients(m11, m12, m22, c, s);
		float cc = c * c;
		float ss = s * s;
		float mix = 2 * c * s * m12;
		SetSymmetric(m00, c * m01 - s * m02, s * m01 + c * m02,
			cc * m11 - mix + ss * m22, 0, ss * m11 + mix + cc * m22);
	}

	coMat3 GetMat3() const
	{
		return coMat3(
			coFloatx3(m00, m01, m02), 
			coFloatx3(m01, m11, m12),
			coFloatx3(m02, m12, m22));
	}
};

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

void Rot01_post(coMat3& this_, float c, float s)
{
	float m00 = this_[0][0], m01 = this_[0][1], m10 = this_[1][0], m11 = this_[1][1], m20 = this_[2][0],
		m21 = this_[2][1];
	this_ = coMat3(
		coFloatx3(c * m00 - s * m01, s * m00 + c * m01, this_[0][2]),
		coFloatx3(c * m10 - s * m11, s * m10 + c * m11, this_[1][2]),
		coFloatx3(c * m20 - s * m21, s * m20 + c * m21, this_[2][2]));
}

void Rot02_post(coMat3& this_, float c, float s)
{
	float m00 = this_[0][0], m02 = this_[0][2], m10 = this_[1][0], m12 = this_[1][2],
		m20 = this_[2][0], m22 = this_[2][2];
	this_ = coMat3(
		coFloatx3(c * m00 - s * m02, this_[0][1], s * m00 + c * m02),
		coFloatx3(c * m10 - s * m12, this_[1][1], s * m10 + c * m12),
		coFloatx3(c * m20 - s * m22, this_[2][1], s * m20 + c * m22));
}

void Rot12_post(coMat3& this_, float c, float s)
{
	float m01 = this_[0][1], m02 = this_[0][2], m11 = this_[1][1], m12 = this_[1][2],
		m21 = this_[2][1], m22 = this_[2][2];
	this_ = coMat3(
		coFloatx3(this_[0][0], c * m01 - s * m02, s * m01 + c * m02),
		coFloatx3(this_[1][0], c * m11 - s * m12, s * m11 + c * m12),
		coFloatx3(this_[2][0], c * m21 - s * m22, s * m21 + c * m22));
}

void coSVD_Rotate01(SMat3& vtav, coMat3& v)
{
	if (vtav.m01 == 0.0f)
		return;

	coFloat c = 0.0f;
	coFloat s = 0.0f;
	vtav.Rot01(c, s);
	c = 0.0f;
	s = 0.0f;
	Rot01_post(v, c, s);
}

void coSVD_Rotate02(SMat3& vtav, coMat3& v)
{
	if (vtav.m02 == 0.0f)
		return;

	coFloat c = 0.0f;
	coFloat s = 0.0f;
	vtav.Rot02(c, s);
	c = 0.0f;
	s = 0.0f;
	Rot02_post(v, c, s);
}

void coSVD_Rotate12(SMat3& vtav, coMat3& v)
{
	if (vtav.m12 == 0)
		return;

	coFloat c = 0.0f;
	coFloat s = 0.0f;
	vtav.Rot12(c, s);
	c = 0.0f;
	s = 0.0f;
	Rot12_post(v, c, s);
}

void coSVD_SolveSym(coVec3& sigma, coMat3& v, const coMat3& a)
{
	// assuming that A is symmetric: can optimize all operations for 
	// the upper right triagonal
	SMat3 vtav(a);

	float delta = 1e-6f * vtav.Fnorm();
	// assuming V is identity: you can also pass a matrix the rotations
	// should be applied to
	// U is not computed
	for (coInt i = 0; i < 4 && vtav.Off() > delta; ++i)
	{
		coSVD_Rotate01(vtav, v);
		coSVD_Rotate02(vtav, v);
		coSVD_Rotate12(vtav, v);
// 		coSVD_Rotate(vtav, v, 0, 1);
// 		coSVD_Rotate(vtav, v, 0, 2);
// 		coSVD_Rotate(vtav, v, 1, 2);
	}
	sigma = coVec3(vtav.m00, vtav.m11, vtav.m22);
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
	btb += (b * b).x;
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
	btb += qef.btb;
	pointAccum += qef.pointAccum;
}

coFloat coQEF::Solve(coVec3& posOut)
{
	coASSERT(pointAccum.w != 0.0f);
	massPoint = coVec3(pointAccum / pointAccum.w);
	const coVec3 ATb2 = ATb - coSVD_VMulSym(ATA, massPoint);
	coSVD_Solve_ATA_ATb(posOut, ATA, ATb2);
	const coFloatx4 error = coQEF_ComputeError(ATA, posOut, ATb2);
	posOut += massPoint;
	return error.x;
}
