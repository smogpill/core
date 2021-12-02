// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../vector/coVec3.h"
#include "../vector/coVec4.h"

class coRay
{
public:
	coRay() = default;
	coRay(const coVec3& origin_, const coVec3& dir_, const coFloatx4& len = coFloatx4(1e20f)) : origin(origin_), dirAndLength(dir_, len) {}

	const coVec3& GetDir() const { return dirAndLength; }

	coVec3 origin;
	coVec4 dirAndLength;
};

class coInfiniteRay : public coRay
{
public:
	coInfiniteRay() = default;
	coInfiniteRay(const coVec3 & origin_, const coVec3& dir_) : coRay(origin_, dir_) {}
};
