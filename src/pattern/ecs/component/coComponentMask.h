// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../coECSConfig.h"

class alignas(16) coComponentMask
{
public:
	coBool operator==(const coComponentMask& m) const;
	coBool operator!=(const coComponentMask& m) const { return !operator==(m); }

	coUint64 masks[co_maxNbComponentsPerWorld / 64] = {};
};

coBool coComponentMask::operator==(const coComponentMask& m) const
{
	for (coUint i = 0; i < coARRAY_SIZE(masks); ++i)
	{
		if (masks[i] != m.masks[i])
			return false;
	}
	return true;
}
