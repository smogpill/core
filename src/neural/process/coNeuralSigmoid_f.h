// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/scalar/coFloat_f.h"

coFORCE_INLINE coFloat coComputeNeuralSigmoid(coFloat _x)
{
	return _x < -45.0f ? 0.0f : _x > 45.0f ? 1.0f : 1.0f / (1.0f + coExp(-_x));

	// Not used for now, because:
	// - Have to find a good derivative form
	// - Have to understand how the range change from [0, 1] to [-1, 1] affects the inputs and outputs.
	// tanh = (exp(2x) - 1) / (exp(2x) + 1)
	// tanh is better than the usual logistic function because it avoids saturation at near 0. 
	// 	const coFloat a = 2.0f * _x;
	// 	const coFloat b = coExp(a);
	// 	return (b - 1.0f) / (b + 1.0f);
}

coFORCE_INLINE coFloat coComputeNeuralSigmoidDerivative(coFloat _x)
{
	return _x * (1 - _x);
}
