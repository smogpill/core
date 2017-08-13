// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/scalar/coFloat_f.h"

#define coNEURAL_LOGISTIC_ACTIVATION 0
#define coNEURAL_TANH_ACTIVATION 1
#define coNEURAL_RESCALED_TANH_ACTIVATION 2 // Version proposed by the Efficient Back-Prop paper by Yann LeCun, to avoid saturation effects.
#define coNEURAL_RELU_ACTIVATION 3
#define coNEURAL_LEAKY_RELU_ACTIVATION 4
#define coNEURAL_CURRENT_ACTIVATION coNEURAL_RESCALED_TANH_ACTIVATION

#if coNEURAL_CURRENT_ACTIVATION == coNEURAL_LOGISTIC_ACTIVATION || coNEURAL_CURRENT_ACTIVATION == coNEURAL_RELU_ACTIVATION || coNEURAL_CURRENT_ACTIVATION == coNEURAL_LEAKY_RELU_ACTIVATION
#define coNEURAL_ACTIVATION_MEAN 0.5f
#define coNEURAL_ACTIVATION_DEVIATION 0.5f
#else 
#define coNEURAL_ACTIVATION_MEAN 0.0f
#define coNEURAL_ACTIVATION_DEVIATION 1.0f
#endif 

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
	return coTanh(_x);
#elif coNEURAL_CURRENT_ACTIVATION == coNEURAL_RESCALED_TANH_ACTIVATION
	return 1.7159f * coTanh(_x * 2.0f / 3.0f);
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
#elif coNEURAL_CURRENT_ACTIVATION == coNEURAL_RESCALED_TANH_ACTIVATION
	return 0.6667f / 1.7159f * (1.7159f - _output) * (1.7159f + _output);
#elif coNEURAL_CURRENT_ACTIVATION == coNEURAL_RELU_ACTIVATION
	return _output > 0.0f ? 1.0f : 0.0f;
#elif coNEURAL_CURRENT_ACTIVATION == coNEURAL_LEAKY_RELU_ACTIVATION
	return _output > 0.0f ? 1.0f : 0.01f;
#endif
}
