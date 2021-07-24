// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "math/vector/coColor.h"
#include "math/vector/coVec4_f.h"

const coColor coColor::s_white(255, 255, 255);
const coColor coColor::s_black(0, 0, 0);
const coColor coColor::s_red(255, 0, 0);
const coColor coColor::s_green(0, 255, 0);
const coColor coColor::s_blue(0, 0, 255);

coColor::operator coVec4() const
{
	return coVec4(r, g, b, a) / coVec4(255.0f);
}
