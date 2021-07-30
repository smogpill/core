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
const coColor coColor::s_coolRed(241, 29, 29);
const coColor coColor::s_coolGreen(61, 230, 59);
const coColor coColor::s_coolBlue(0, 155, 178);
const coColor coColor::s_coolOrange(255, 113, 0);

coColor::operator coVec4() const
{
	return coVec4(r, g, b, a) / coVec4(255.0f);
}
