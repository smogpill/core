// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coSVD_f.h"
#include "../vector/coVec4_f.h"
#include "../matrix/coMat4_f.h"

void coComputeSVD(coVec4& sigma, coMat4& VOut, const coMat4& A)
{
	coMat4 U = A;
	coMat4 V;

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

				const coVec4 vi = V[i];
				V[i] = c*vi - s*V[j];
				V[j] = s*vi + c*V[j];
			}
		}
	}

	for (coUint i = 0; i < 4; ++i)
	{
		sigma[i] = coLength(U[i]).x;
	}

	VOut = V;
}

void coSVD_ComputePseudoInverse(coMat4& out, const coVec4& sigma, const coMat4& v)
{
	const coFloat tolerance = 1e-6f;
	coVec4 invSigma;
	invSigma.x = (coAbs(sigma.x) < tolerance || coAbs(1.0f / sigma.x) < tolerance) ? 0.0f : (1.0f / sigma.x);
	invSigma.y = (coAbs(sigma.y) < tolerance || coAbs(1.0f / sigma.y) < tolerance) ? 0.0f : (1.0f / sigma.y);
	invSigma.z = (coAbs(sigma.z) < tolerance || coAbs(1.0f / sigma.z) < tolerance) ? 0.0f : (1.0f / sigma.z);
	invSigma.w = (coAbs(sigma.w) < tolerance || coAbs(1.0f / sigma.w) < tolerance) ? 0.0f : (1.0f / sigma.w);
	out = v * coMat4(invSigma) * coTranspose(v);
}

void coSolveBySymmetricSVD(coVec4& out, const coMat4& SymA, const coVec4& b)
{
	coMat4 V;
	coVec4 sigma(nullptr);
	coComputeSVD(sigma, V, SymA);
	coMat4 invV(nullptr);
	coSVD_ComputePseudoInverse(invV, sigma, V);
	out = invV * b;
}
