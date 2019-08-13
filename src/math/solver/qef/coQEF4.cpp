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
// Docs:
// - https://web.stanford.edu/class/cme335/lecture7.pdf

const coFloat coQEF4::tolerance = 1e-6f;

void coSVD_SolveSym2(coVec4& sigma, coMat4& V, const coMat4& A)
{
	coMat4 U = A;
	coMat4 v2;

	coFloat converge = 777.0f;
	for (coUint it = 0; it < 40; ++it)
	//while (converge > 0.0001f)
	{
		converge = 0.0f;
		for (coUint i = 1; i < 4; ++i)
		{
			for (coUint j = 0; j < i; ++j)
			{
				const coFloat alpha = coSquareLength(U[i]).x;
				const coFloat beta = coSquareLength(U[j]).x;
				const coFloat gamma = coDot(U[i], U[j]).x;
				converge = coMax(converge, coAbs(gamma) / coSquareRoot(alpha*beta));

				if (coAbs(gamma) < 1e-12f)
				{
					continue;
				}
				const coFloat zeta = (beta - alpha) / (2.0f * gamma);
				const coFloat t = coSign(zeta) / (coAbs(zeta) + coSquareRoot(1.0f + (zeta*zeta)));
				const coFloat c = 1.0f / coSquareRoot(1.0f + (t*t));
				const coFloat s = c * t;

				const coVec4 ui = U[i];
				U[i] = c*ui - s*U[j];
				U[j] = s*ui + c*U[j];

				const coVec4 vi = v2[i];
				v2[i] = c*vi - s*v2[j];
				v2[j] = s*vi + c*v2[j];
			}
		}
	}

	for (coUint i = 0; i < 4; ++i)
	{
		sigma[i] = coLength(U[i]).x;
	}

	V = v2;
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

void coSVD_SolveSymmetric(coVec4& out, const coMat4& A, const coVec4& b)
{
	coMat4 V;
	coVec4 sigma(nullptr);
	coSVD_SolveSym2(sigma, V, A);
	coMat4 invV(nullptr);
	coSVD_ComputePseudoInverse(invV, sigma, V);
	out = invV * b;
}

coVec4 coQEF4::Solve()
{
	coASSERT(nb);
	massPoint = pointAccum / coFloat(nb);
	const coVec4 ATb2 = ATb - ATA * massPoint;
	coVec4 x;
	coSVD_SolveSymmetric(x, ATA, ATb2);
	return x + massPoint;
}

coFloatx4 coQEF4::ComputeError(const coVec4& x) const
{
	// E(x) = (Ax-b)T(Ax-b)
	//      = x.ATAx - 2x.ATb + btb
	const coVec4 atax = ATA * x;
	return coDot(x, atax) - 2.0f * coDot(x, ATb) + btb;
}

void coQEF4::Add(const coVec4& p, const coVec4& n)
{
	const coVec4 nx = coBroadcastX(n);
	const coVec4 ny = coBroadcastY(n);
	const coVec4 nz = coBroadcastZ(n);
	const coVec4 nw = coBroadcastW(n);
	ATA.c0 += nx * n;
	ATA.c1 += ny * n;
	ATA.c2 += nz * n;
	ATA.c3 += nw * n;
	const coFloatx4 d = coDot(p, n);
	ATb += n * d;
	btb += (p * p).x;
	pointAccum += p;
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
