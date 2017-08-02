// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/scalar/coFloat_f.h"

coFORCE_INLINE coFloat coComputeNeuralActivation(coFloat _x)
{
	return 1.0f / (1.0f + coExp(-_x));

	// Not used for now, because:
	// - Have to find a good derivative form
	// - Have to understand how the range change from [0, 1] to [-1, 1] affects the inputs and outputs.
	// tanh = (exp(2x) - 1) / (exp(2x) + 1)
	// tanh is better than the usual logistic function because it avoids saturation at near 0. 
	// 	const coFloat a = 2.0f * _x;
	// 	const coFloat b = coExp(a);
	// 	return (b - 1.0f) / (b + 1.0f);
}

// Not the direct activation derivative, this takes the output as a parameter
coFORCE_INLINE coFloat coComputeNeuralActivationDerivativeTransfer(coFloat _output)
{
	// The derivative is coComputeNeuralActivation(x) * (1 - coComputeNeuralActivation(x))
	return _output * (1 - _output);
}
