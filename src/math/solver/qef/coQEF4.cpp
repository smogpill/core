// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "coQEF4.h"
#include "../coSVD_f.h"
#include "../../matrix/coSymMat4_f.h"
#include "../../matrix/coMat4_f.h"
#include "../../vector/coVec4_f.h"

// Docs:
// - https://www.mattkeeter.com/projects/qef/
// - https://web.stanford.edu/class/cme335/lecture7.pdf

coVec4 coQEF4::Solve()
{
	coASSERT(nb);
	massPoint /= coFloat(nb);
	const coVec4 ATb2 = ATb - ATA * massPoint;
	coVec4 x;
	coSolveBySymmetricSVD(x, ATA, ATb2);
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
	massPoint += p;
	++nb;
}

void coQEF4::Add(const coQEF4& qef)
{
	ATA += qef.ATA;
	ATb += qef.ATb;
	btb += qef.btb;
	massPoint += qef.massPoint;
	nb += qef.nb;
}
