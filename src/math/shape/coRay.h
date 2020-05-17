// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../vector/coVec3.h"

class coRay
{
public:
	coRay() = default;
	coRay(const coVec3& origin_, const coVec3& dir_): origin(origin_), dir(dir_) {}
	coVec3 origin;
	coVec3 dir;
};
