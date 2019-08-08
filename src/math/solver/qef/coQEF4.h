// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <math/matrix/coMat4.h>
#include <math/vector/coVec4.h>

class coQEF4
{
public:
	void Add(const coQEF4& qef);
	void Add(const coVec4& pos, const coVec4& normal);
	coVec4 Solve();
	coFloatx4 ComputeError(const coVec4& x) const;
	const coVec4& GetLastMassPoint() const { return massPoint; }

	static const coFloat tolerance;

private:
	coMat4 ATA = coMat4(0.0f);
	coVec4 ATb = coVec4(0.0f);
	coVec4 massPoint = coVec4(0.0f);
	coVec4 pointAccum = coVec4(0.0f);
	coUint nb = 0;
	coFloat btb = 0.0f;
};
