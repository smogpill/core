// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "neural/pch.h"
#include "neural/process/compute/coNeuralComputeOutputs.h"
#include "neural/process/coNeuralSigmoid_f.h"
#include "neural/network/coNeuralLayer.h"

void coComputeNeuralLayerOutputs(const coNeuralLayer& _layerData, const coArray<coFloat>& _inputs, coArray<coFloat>& _outputs)
{
	const coUint nbInputs = _layerData.GetNbInputs();
	const coUint nbOutputs = _layerData.GetNbOutputs();
	coASSERT(_inputs.count == nbInputs);
	coASSERT(_outputs.count == nbOutputs);

	const coArray<coFloat>& weightBuffer = _layerData.GetWeightBuffer();
	const coArray<coFloat>& biasBuffer = _layerData.GetBiasBuffer();

	coUint weightIndex = 0;
	for (coUint j = 0; j < nbOutputs; ++j)
	{
		coFloat o = biasBuffer[j];
		for (coUint i = 0; i < nbInputs; ++i)
		{
			o += _inputs[i] * weightBuffer[weightIndex];
			++weightIndex;
		}
		_outputs[j] = coComputeNeuralSigmoid(o);
	}
}
