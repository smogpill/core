// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "math/vector/coVec3.h"
#include "math/scalar/coFloat_f.h"

coVec3::coVec3(coNullPtr)
{
#ifdef coDEBUG
	x = coFloat_qNaN;
	y = coFloat_qNaN;
	z = coFloat_qNaN;
	pad = coFloat_qNaN;
#endif
}
