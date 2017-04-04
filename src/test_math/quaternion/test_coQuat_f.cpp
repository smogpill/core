// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_math/pch.h"
#include "test/unit/coTest.h"
#include "math/quaternion/coQuat_f.h"

coTEST(coQuat_f, coNormalize)
{
	coEXPECT(coNearEqual(coNormalize(coQuat(-7, 0, 0, 0)), coQuat(-1, 0, 0, 0)));
	coEXPECT(coNearEqual(coNormalize(coQuat(0, -7, 0, 0)), coQuat(0, -1, 0, 0)));
	coEXPECT(coNearEqual(coNormalize(coQuat(0, 0, -7, 0)), coQuat(0, 0, -1, 0)));
	coEXPECT(coNearEqual(coNormalize(coQuat(0, 0, 0, -7)), coQuat(0, 0, 0, -1)));
}

coTEST(coQuat_f, coConjugate)
{
	coEXPECT(coConjugate(coQuat(0, 0, 0, 0)) == coQuat(0, 0, 0, 0));
	coEXPECT(coConjugate(coQuat(7, 0, 0, 0)) == coQuat(-7, 0, 0, 0));
	coEXPECT(coConjugate(coQuat(0, 7, 0, 0)) == coQuat(0, -7, 0, 0));
	coEXPECT(coConjugate(coQuat(0, 0, 7, 0)) == coQuat(0, 0, -7, 0));  
	coEXPECT(coConjugate(coQuat(0, 0, 0, 7)) == coQuat(0, 0, 0, -7));
}

coTEST(coQuat_f, coRotation)
{
	coEXPECT(coNearEqual(coRotation(coFloatx3(7, 0, 0), coFloatx3(7, 0, 0)), coRotation(coFloatx3(0, 0, 0))));
	coEXPECT(coNearEqual(coRotation(coFloatx3(7, 0, 0), coFloatx3(0, 7, 0)), coRotation(coFloatx3(0, 0, coFloat_halfPi))));
	coEXPECT(coNearEqual(coRotation(coFloatx3(7, 0, 0), coFloatx3(0, 0, 7)), coRotation(coFloatx3(0, coFloat_halfPi, 0))));
	coEXPECT(coNearEqual(coRotation(coFloatx3(0, 0, 7), coFloatx3(0, 7, 0)), coRotation(coFloatx3(coFloat_halfPi, 0, 0))));
}

coTEST(coQuat_f, coRotateVector)
{
	{
		const coQuat q = coRotation(coFloatx3(0, 0, 0));
		coEXPECT(coNearEqual(coRotateVector(q, coFloatx3(3, 0, 0)), coFloatx3(3, 0, 0)));
		coEXPECT(coNearEqual(coRotateVector(q, coFloatx3(0, 3, 0)), coFloatx3(0, 3, 0)));
		coEXPECT(coNearEqual(coRotateVector(q, coFloatx3(0, 0, 3)), coFloatx3(0, 0, 3)));
	}

	{
		const coQuat q = coRotation(coFloatx3(coFloat_halfPi, 0, 0));
		coEXPECT(coNearEqual(coRotateVector(q, coFloatx3(3, 0, 0)), coFloatx3(3, 0, 0)));
		coEXPECT(coNearEqual(coRotateVector(q, coFloatx3(0, 3, 0)), coFloatx3(0, 0, -3)));
		coEXPECT(coNearEqual(coRotateVector(q, coFloatx3(0, 0, 3)), coFloatx3(0, 3, 0)));
	}

	{
		const coQuat q = coRotation(coFloatx3(0, coFloat_halfPi, 0));
		coEXPECT(coNearEqual(coRotateVector(q, coFloatx3(3, 0, 0)), coFloatx3(0, 0, 3)));
		coEXPECT(coNearEqual(coRotateVector(q, coFloatx3(0, 3, 0)), coFloatx3(0, 3, 0)));
		coEXPECT(coNearEqual(coRotateVector(q, coFloatx3(0, 0, 3)), coFloatx3(-3, 0, 0)));
	}
}
