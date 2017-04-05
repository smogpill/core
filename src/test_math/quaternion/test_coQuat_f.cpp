// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_math/pch.h"
#include "test/unit/coTest.h"
#include "math/quaternion/coQuat_f.h"

coTEST(coQuat_f, coNormalize)
{
	coEXPECT(coNearEqual(coNormalize(coQuat()), coQuat()));
	coEXPECT(coNearEqual(coNormalize(coQuat(-7, 0, 0, 0)), coQuat(-1, 0, 0, 0)));
	coEXPECT(coNearEqual(coNormalize(coQuat(0, -7, 0, 0)), coQuat(0, -1, 0, 0)));
	coEXPECT(coNearEqual(coNormalize(coQuat(0, 0, -7, 0)), coQuat(0, 0, -1, 0)));
	coEXPECT(coNearEqual(coNormalize(coQuat(0, 0, 0, -7)), coQuat(0, 0, 0, -1)));
}

coTEST(coQuat_f, coInverse)
{
	coEXPECT(coInverse(coQuat()) == coQuat());
	coEXPECT(coInverse(coQuat(0, 0, 0, 0)) == coQuat(0, 0, 0, 0));
	coEXPECT(coInverse(coQuat(1, 2, 3, 4)) == coQuat(-1, -2, -3, 4));
	coEXPECT(coInverse(coQuat(7, 0, 0, 0)) == coQuat(-7, 0, 0, 0));
	coEXPECT(coInverse(coQuat(0, 7, 0, 0)) == coQuat(0, -7, 0, 0));
	coEXPECT(coInverse(coQuat(0, 0, 7, 0)) == coQuat(0, 0, -7, 0));
	coEXPECT(coInverse(coQuat(0, 0, 0, 7)) == coQuat(0, 0, 0, 7));
}

coTEST(coQuat_f, coRotation_FromEuler)
{
	coEXPECT(coNearEqual(coRotation(coFloatx3(0)), coQuat()));
}

coTEST(coQuat_f, coRotation_AxisAngle)
{
	coEXPECT(coNearEqual(coRotation(coFloatx3(1, 0, 0), 0), coQuat()));
	coEXPECT(coNearEqual(coRotation(coFloatx3(0, 1, 0), 0), coQuat()));
	coEXPECT(coNearEqual(coRotation(coFloatx3(0, 0, 1), 0), coQuat()));

	coEXPECT(coNearEqual(coRotation(coFloatx3(1, 0, 0), coFloat_halfPi), coRotation(coFloatx3(coFloat_halfPi, 0, 0))));
	coEXPECT(coNearEqual(coRotation(coFloatx3(-1, 0, 0), coFloat_halfPi), coRotation(coFloatx3(-coFloat_halfPi, 0, 0))));
	coEXPECT(coNearEqual(coRotation(coFloatx3(0, 1, 0), coFloat_halfPi), coRotation(coFloatx3(0, coFloat_halfPi, 0))));
	coEXPECT(coNearEqual(coRotation(coFloatx3(0, -1, 0), coFloat_halfPi), coRotation(coFloatx3(0, -coFloat_halfPi, 0))));
	coEXPECT(coNearEqual(coRotation(coFloatx3(0, 0, 1), coFloat_halfPi), coRotation(coFloatx3(0, 0, coFloat_halfPi))));
	coEXPECT(coNearEqual(coRotation(coFloatx3(0, 0, -1), coFloat_halfPi), coRotation(coFloatx3(0, 0, -coFloat_halfPi))));
}

coTEST(coQuat_f, coRotation_VectorToVector)
{
	coEXPECT(coNearEqual(coRotation(coFloatx3(7, 0, 0), coFloatx3(7, 0, 0)), coRotation(coFloatx3(0, 0, 0))));
	coEXPECT(coNearEqual(coRotation(coFloatx3(7, 0, 0), coFloatx3(0, 7, 0)), coRotation(coFloatx3(0, 0, coFloat_halfPi))));
	coEXPECT(coNearEqual(coRotation(coFloatx3(7, 0, 0), coFloatx3(0, 0, 7)), coRotation(coFloatx3(0, -coFloat_halfPi, 0))));
	coEXPECT(coNearEqual(coRotation(coFloatx3(0, 0, 7), coFloatx3(0, 7, 0)), coRotation(coFloatx3(-coFloat_halfPi, 0, 0))));
}

coTEST(coQuat_f, operatorMultiply)
{
	const coQuat rx = coRotation(coFloatx3(coFloat_halfPi, 0, 0));
	const coQuat ry = coRotation(coFloatx3(0, coFloat_halfPi, 0));
	const coQuat rz = coRotation(coFloatx3(0, 0, coFloat_halfPi));
	coEXPECT(coQuat() * coQuat() == coQuat());
	coEXPECT(coNearEqual(coNormalize(rx * rx), coRotation(coFloatx3(coFloat_pi, 0, 0))));
	coEXPECT(coNearEqual(coNormalize(ry * ry), coRotation(coFloatx3(0, coFloat_pi, 0))));
	coEXPECT(coNearEqual(coNormalize(rz * rz), coRotation(coFloatx3(0, 0, coFloat_pi))));
}

coTEST(coQuat_f, coRotateVector)
{
	const coFloatx3 epsilon = 0.01f;

	{
		const coQuat q;
		coEXPECT(coNearEqual(coRotateVector(q, coFloatx3(3, 0, 0)), coFloatx3(3, 0, 0), epsilon));
		coEXPECT(coNearEqual(coRotateVector(q, coFloatx3(0, 3, 0)), coFloatx3(0, 3, 0), epsilon));
		coEXPECT(coNearEqual(coRotateVector(q, coFloatx3(0, 0, 3)), coFloatx3(0, 0, 3), epsilon));
	}

	{
		const coQuat q = coRotation(coFloatx3(coFloat_halfPi, 0, 0));
		coEXPECT(coNearEqual(coRotateVector(q, coFloatx3(3, 0, 0)), coFloatx3(3, 0, 0), epsilon));
		coEXPECT(coNearEqual(coRotateVector(q, coFloatx3(0, 3, 0)), coFloatx3(0, 0, 3), epsilon));
		coEXPECT(coNearEqual(coRotateVector(q, coFloatx3(0, 0, 3)), coFloatx3(0, -3, 0), epsilon));
	}

	{
		const coQuat q = coRotation(coFloatx3(0, coFloat_halfPi, 0));
		coEXPECT(coNearEqual(coRotateVector(q, coFloatx3(3, 0, 0)), coFloatx3(0, 0, -3), epsilon));
		coEXPECT(coNearEqual(coRotateVector(q, coFloatx3(0, 3, 0)), coFloatx3(0, 3, 0), epsilon));
		coEXPECT(coNearEqual(coRotateVector(q, coFloatx3(0, 0, 3)), coFloatx3(3, 0, 0), epsilon));
	}
}

coTEST(coQuat_f, coInverseRotateVector)
{
	const coFloatx3 epsilon = 0.01f;

	{
		const coQuat q;
		coEXPECT(coNearEqual(coInverseRotateVector(q, coFloatx3(3, 0, 0)), coFloatx3(3, 0, 0), epsilon));
		coEXPECT(coNearEqual(coInverseRotateVector(q, coFloatx3(0, 3, 0)), coFloatx3(0, 3, 0), epsilon));
		coEXPECT(coNearEqual(coInverseRotateVector(q, coFloatx3(0, 0, 3)), coFloatx3(0, 0, 3), epsilon));
	}

	{
		const coQuat q = coRotation(coFloatx3(coFloat_halfPi, 0, 0));
		coEXPECT(coNearEqual(coInverseRotateVector(q, coFloatx3(3, 0, 0)), coFloatx3(3, 0, 0), epsilon));
		coEXPECT(coNearEqual(coInverseRotateVector(q, coFloatx3(0, 3, 0)), coFloatx3(0, 0, -3), epsilon));
		coEXPECT(coNearEqual(coInverseRotateVector(q, coFloatx3(0, 0, 3)), coFloatx3(0, 3, 0), epsilon));
	}

	{
		const coQuat q = coRotation(coFloatx3(0, coFloat_halfPi, 0));
		coEXPECT(coNearEqual(coInverseRotateVector(q, coFloatx3(3, 0, 0)), coFloatx3(0, 0, 3), epsilon));
		coEXPECT(coNearEqual(coInverseRotateVector(q, coFloatx3(0, 3, 0)), coFloatx3(0, 3, 0), epsilon));
		coEXPECT(coNearEqual(coInverseRotateVector(q, coFloatx3(0, 0, 3)), coFloatx3(-3, 0, 0), epsilon));
	}
}
