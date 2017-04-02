// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "math/quaternion/coQuat.h"
#include "math/scalar/coFloat_f.h"

coQuat::coQuat(coNullPtr)
#ifdef coDEBUG
	: x(coFloat_qNaN)
	, y(coFloat_qNaN)
	, z(coFloat_qNaN)
	, w(coFloat_qNaN)
#endif
{

}
