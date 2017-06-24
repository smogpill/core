// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "math/vector/coFloatx3.h"

coFloatx3::coFloatx3()
{
#ifdef coDEBUG
	coBitCast<__m128>(*this) = _mm_set_ps1(std::numeric_limits<float>::signaling_NaN());
	pad = std::numeric_limits<float>::quiet_NaN();
#endif
}
