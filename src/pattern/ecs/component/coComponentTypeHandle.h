// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../coECSConfig.h"

class coComponentTypeHandle
{
public:
	static const coComponentTypeHandle invalid;

public:
	coComponentTypeHandle() = default;
	coComponentTypeHandle(const coUint index);
	coBool IsValid() const { return index < co_maxNbComponentsPerWorld; }
	coBool operator==(const coComponentTypeHandle h) const { return index == h.index; }
	coBool operator!=(const coComponentTypeHandle h) const { return index != h.index; }

	coUint16 index = coUint16(-1);
};
