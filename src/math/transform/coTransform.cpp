// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "math/transform/coTransform.h"
#include "math/quaternion/coQuat.h"
#include "math/scalar/coFloat_f.h"

coTransform::coTransform()
	: translation(0.0f)
	, scale(1.0f)
{

}

coTransform::coTransform(coNullPtr)
	: rotation(nullptr)
#ifdef coDEBUG
	, translation(coFloat_qNaN)
	, scale(coFloat_qNaN)
#endif
{

}
