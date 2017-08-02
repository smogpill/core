// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "neural/pch.h"
#include "neural/process/coNeuralLayer_f.h"
#include "neural/process/coNeuralActivation_f.h"
#include "neural/network/coNeuralLayer.h"
#include "math/scalar/coFloat_f.h"

void coResetWeightsAndBiases(coNeuralLayer& _this, coUint32& _seed)
{
	// Xavier initialization (Caffe library).
	// Original paper: http://proceedings.mlr.press/v9/glorot10a/glorot10a.pdf
	const coFloat scale = coInvSqrt(coFloat(_this.GetNbInputs()));
	for (coFloat& weight : _this.GetWeightBuffer())
	{
		weight = coRand11(_seed) * scale;
	}
	for (coFloat& bias : _this.GetBiasBuffer())
	{
		bias = 0.0f;
	}
}

void coComputeNeuralOutputs(const coNeuralLayer& _this, const coArray<coFloat>& _inputs, coArray<coFloat>& _outputs)
{
	const coUint nbInputs = _this.GetNbInputs();
	const coUint nbOutputs = _this.GetNbOutputs();
	coASSERT(_inputs.count == nbInputs);
	coASSERT(_outputs.count == nbOutputs);

	const coArray<coFloat>& weightBuffer = _this.GetWeightBuffer();
	const coArray<coFloat>& biasBuffer = _this.GetBiasBuffer();

	coUint weightIndex = 0;
	for (coUint j = 0; j < nbOutputs; ++j)
	{
		coFloat o = biasBuffer[j];
		for (coUint i = 0; i < nbInputs; ++i)
		{
			o += _inputs[i] * weightBuffer[weightIndex];
			++weightIndex;
		}
		_outputs[j] = coComputeNeuralActivation(o);
	}
}
