// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coBitMat88.h"
#include "../vector/coUint64x4.h"

class coBitMat88
{
public:
	coBitMat88();
	coBitMat88(const coUint64 v) : xy(v) {}

	coUint64 xy;
};
