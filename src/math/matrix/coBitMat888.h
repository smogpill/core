// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../vector/coUint64x4.h"
#include "coBitMat88.h"

class alignas(32) coBitMat888
{
public:
	coBool operator()(coUint x, coUint y, coUint z) const;
	void operator=(coBool b);

	coUint64x4 _h[2] = {};
};
