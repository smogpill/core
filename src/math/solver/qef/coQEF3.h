// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <math/matrix/coMat3.h>
#include <math/vector/coVec3.h>

class coQEF3
{
public:
	void Add(const coQEF3& qef);
	void Add(const coVec3& pos, const coVec3& normal);
	coFloat Solve(coVec3& posOut);
	const coVec3& GetMassPoint() const { return massPoint; }

	static const coFloat tolerance;

private:
	coMat3 ATA = coMat3(0.0f);
	coVec3 ATb = coVec3(0.0f);
	coFloat btb = 0.0f;
	coVec3 pointAccum = coVec3(0.0f);
	coUint nbPoints = 0;
	coVec3 massPoint = coVec3(0.0f);
};
