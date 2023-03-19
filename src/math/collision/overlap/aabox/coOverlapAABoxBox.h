// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../../../shape/coAabb.h"
#include "../../../matrix/coMat4.h"
#include "../../../vector/coVec3_f.h"

coBool coOverlap_AABox_Box(const coAabb& aabox, const coMat4& box);

coFORCE_INLINE coBool coOverlap_AABox_Box(const coAabb& aabox, const coMat4& box)
{
	coASSERT(false);
	return true;
}