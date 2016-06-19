// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/pch.h"
#include "math/quaternion/Quat_f.h"

namespace co
{
	void squadSetup(Quat& _out0, Quat& _out1, Quat& _out2, const Quat& _q0, const Quat& _q1, const Quat& _q2, const Quat& _q3)
	{
		Quat q1 = _q1;
		Quat q2 = _q2;
		Quat q3 = _q3;

		q1 = syncWith(q1, _q0);
		q2 = syncWith(q2, q1);
		q3 = syncWith(q3, q2);

		const Quat q1Conj = conjugate(q1);
		const Quat q2Conj = conjugate(q2);

		const Floatx4 c = make_floatx4(-0.25f);
		_out0 = q1 * exp((ln(q1Conj*q2) + ln(q1Conj*_q0)) * c);
		_out1 = q2 * exp((ln(q2Conj*q3) + ln(q2Conj*q1)) * c);
		_out2 = q2;
	}
}
