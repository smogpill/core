// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/pch.h"
#include "math/quaternion/quat_f.h"

namespace co
{
	void squadSetup(quat& _out0, quat& _out1, quat& _out2, const quat& _q0, const quat& _q1, const quat& _q2, const quat& _q3)
	{
		quat q1 = _q1;
		quat q2 = _q2;
		quat q3 = _q3;

		q1 = syncWith(q1, _q0);
		q2 = syncWith(q2, q1);
		q3 = syncWith(q3, q2);

		const quat q1Conj = conjugate(q1);
		const quat q2Conj = conjugate(q2);

		const floatx4 c = make_floatx4(-0.25f);
		_out0 = q1 * exp((ln(q1Conj*q2) + ln(q1Conj*_q0)) * c);
		_out1 = q2 * exp((ln(q2Conj*q3) + ln(q2Conj*q1)) * c);
		_out2 = q2;
	}
}
