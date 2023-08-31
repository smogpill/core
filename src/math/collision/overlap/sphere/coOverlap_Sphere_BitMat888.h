// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../../../matrix/coBitMat888_f.h"
#include "../../../vector/coVec3_f.h"
#include "../aabox/coOverlap_AABox_Sphere.h"

inline coBitMat888 coOverlapSolidSolid888(const coSphere& sphere, coFloat matCellSize)
{
	coBitMat888 mask;
	coAabb aabb;
	const coVec3 cellSize = coVec3(matCellSize);
	for (coUint z = 0; z < 8; ++z)
	{
		for (coUint y = 0; y < 8; ++y)
		{
			for (coUint x = 0; x < 8; ++x)
			{
				aabb.min = coVec3(coFloat(x), coFloat(y), coFloat(z)) * cellSize;
				aabb.max = aabb.min + cellSize;
				const coBool overlaps = coAreAllTrue(coOverlapSolidSolid(aabb, sphere));
				coSetElement(mask, x, y, z, overlaps);
			}
		}
	}
	return mask;
}
