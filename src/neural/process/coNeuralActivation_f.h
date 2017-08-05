// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/scalar/coFloat_f.h"

#define coNEURAL_LOGISTIC_ACTIVATION 0
#define coNEURAL_TANH_ACTIVATION 1
#define coNEURAL_RELU_ACTIVATION 2
#define coNEURAL_LEAKY_RELU_ACTIVATION 3
#define coNEURAL_CURRENT_ACTIVATION coNEURAL_LOGISTIC_ACTIVATION

coFORCE_INLINE coFloat coComputeNeuralActivation(coFloat _x)
{
#if coNEURAL_CURRENT_ACTIVATION == coNEURAL_LOGISTIC_ACTIVATION
	// Logistic
	// (output: [0, 1], not good)
	return 1.0f / (1.0f + coExp(-_x));
#elif coNEURAL_CURRENT_ACTIVATION == coNEURAL_TANH_ACTIVATION
	// Tanh is better than the usual logistic function because it avoids saturation at near 0. 
	// (output: [-1, 1])
	//return 2.0f / (1.0f + coExp(-2.0f * _x)) - 1.0f;
	//const coFloat a = coExp(2.0f * _x);
	//return (a - 1.0f) / (a + 1.0f);
	return ::tanhf(_x);
#elif coNEURAL_CURRENT_ACTIVATION == coNEURAL_RELU_ACTIVATION
	return coMax(0.0f, _x);
#elif coNEURAL_CURRENT_ACTIVATION == coNEURAL_LEAKY_RELU_ACTIVATION
	return _x > 0.0f ? _x : 0.01f * _x;
#endif
}

// Not the direct activation derivative, this takes the output as a parameter. Caching trick.
coFORCE_INLINE coFloat coComputeNeuralActivationDerivativeTransfer(coFloat _output)
{
#if coNEURAL_CURRENT_ACTIVATION == coNEURAL_LOGISTIC_ACTIVATION
	// The derivative is coComputeNeuralActivation(x) * (1 - coComputeNeuralActivation(x)) 
	return _output * (1 - _output); // logistic
#elif coNEURAL_CURRENT_ACTIVATION == coNEURAL_TANH_ACTIVATION
	// The derivative is 1 - (coComputeNeuralActivation(x))^2
	return 1.0f - _output * _output; // tanh
#elif coNEURAL_CURRENT_ACTIVATION == coNEURAL_RELU_ACTIVATION
	return _output > 0.0f ? 1.0f : 0.0f;
#elif coNEURAL_CURRENT_ACTIVATION == coNEURAL_LEAKY_RELU_ACTIVATION
	return _output > 0.0f ? 1.0f : 0.01f;
#endif
}
