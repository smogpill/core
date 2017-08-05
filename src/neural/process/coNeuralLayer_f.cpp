// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "neural/pch.h"
#include "neural/process/coNeuralLayer_f.h"
#include "neural/process/coNeuralActivation_f.h"
#include "neural/network/coNeuralLayer.h"
#include "math/scalar/coFloat_f.h"

void coResetWeightsAndBiases(coNeuralLayer& _this, coUint32& _seed)
{
	// Xavier normalized initialization (Glorot & Bengio paper).
	// Original paper: http://proceedings.mlr.press/v9/glorot10a/glorot10a.pdf
	const coFloat scale = coInvSqrt(6.f / coFloat(_this.GetNbInputs() + _this.GetNbOutputs()));
	for (coFloat& weight : _this.GetWeightBuffer())
	{
		weight = coRand11(_seed) * scale;
	}
	coFill(_this.GetBiasBuffer(), 0.0f);
}

void coComputeOutputs(const coNeuralLayer& _this, const coArray<coFloat>& _inputs, coArray<coFloat>& _outputs)
{
	coASSERT(_inputs.count == _this.GetNbInputs());
	coASSERT(_outputs.count == _this.GetNbOutputs());

	// Keep this optimized
	{
		const coUint nbInputs = _this.GetNbInputs();
		const coUint nbOutputs = _this.GetNbOutputs();
		const coFloat* weights = _this.GetWeightBuffer().data;
		const coFloat* bias = _this.GetBiasBuffer().data;
		const coFloat* inputs = _inputs.data;
		coFloat* outputs = _outputs.data;

		coUint weightIndex = 0;
		for (coUint j = 0; j < nbOutputs; ++j)
		{
			coFloat o = bias[j];
			for (coUint i = 0; i < nbInputs; ++i)
			{
				o += inputs[i] * weights[weightIndex];
				++weightIndex;
			}
			outputs[j] = coComputeNeuralActivation(o);
		}
	}
}
