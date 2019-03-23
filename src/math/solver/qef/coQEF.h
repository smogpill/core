// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <math/matrix/coMat3.h>
#include <math/vector/coVec3.h>
#include <math/vector/coVec4.h>

class coQEF
{
public:
	void AddPoint(const coVec3& pointPos, const coVec3& pointNormal);
	coFloat Solve(coVec3& posOut);
	const coVec3& GetMassPoint() const { return massPoint; }

private:
	coMat3 ATA = coMat3(0.0f);
	coVec3 ATb = coVec3(0.0f);
	coVec4 pointAccum = coVec4(0.0f);
	coVec3 pos = coVec3(0.0f);
	coVec3 massPoint = coVec3(0.0f);
};