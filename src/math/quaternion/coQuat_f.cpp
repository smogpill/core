// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/pch.h"
#include "math/quaternion/coQuat_f.h"

void coSetupSquad(coQuat& _out0, coQuat& _out1, coQuat& _out2, const coQuat& _q0, const coQuat& _q1, const coQuat& _q2, const coQuat& _q3)
{
	coQuat q1 = _q1;
	coQuat q2 = _q2;
	coQuat q3 = _q3;

	q1 = coSyncWith(q1, _q0);
	q2 = coSyncWith(q2, q1);
	q3 = coSyncWith(q3, q2);

	const coQuat q1Conj = coConjugate(q1);
	const coQuat q2Conj = coConjugate(q2);

	const coFloatx4 c = coMake_floatx4(-0.25f);
	_out0 = q1 * coExp((coLn(q1Conj*q2) + coLn(q1Conj*_q0)) * c);
	_out1 = q2 * coExp((coLn(q2Conj*q3) + coLn(q2Conj*q1)) * c);
	_out2 = q2;
}
