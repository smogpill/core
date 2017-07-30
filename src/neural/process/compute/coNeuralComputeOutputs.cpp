// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "neural/pch.h"
#include "neural/process/compute/coNeuralComputeOutputs.h"
#include "neural/process/coNeuralSigmoid_f.h"
#include "neural/network/coNeuralNet.h"
#include "neural/network/coNeuralLayer.h"

void coComputeNeuralOutputs(const coNeuralLayer& _layer, const coArray<coFloat>& _inputs, coArray<coFloat>& _outputs)
{
	const coUint nbInputs = _layer.GetNbInputs();
	const coUint nbOutputs = _layer.GetNbOutputs();
	coASSERT(_inputs.count == nbInputs);
	coASSERT(_outputs.count == nbOutputs);

	const coArray<coFloat>& weightBuffer = _layer.GetWeightBuffer();
	const coArray<coFloat>& biasBuffer = _layer.GetBiasBuffer();

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

void coComputeNeuralOutputs(const coNeuralNet& _net, const coArray<coFloat>& _inputs, coArray<coFloat>& _outputs)
{
	const coArray<coNeuralLayer*> layers = _net.GetLayers();
	coASSERT(layers.count > 0);
	coASSERT(layers[0]->GetNbInputs() == _inputs.count);
	coASSERT(coBack(layers)->GetNbOutputs() == _outputs.count);

	// Compute max nb outputs
	coUint nbMaxOutputs = 0;
	for (const coNeuralLayer* layer : layers)
	{
		nbMaxOutputs = coMax(nbMaxOutputs, layer->GetNbOutputs());
	}

	coDynamicArray<coFloat> tempInputs;
	coDynamicArray<coFloat> tempOutputs;
	coResize(tempInputs, nbMaxOutputs);
	coResize(tempOutputs, nbMaxOutputs);

	const coArray<coFloat>* in = &_inputs;
	coArray<coFloat>* out = &tempOutputs;

	for (const coNeuralLayer* layer : layers)
	{
		coArray<coFloat> inputs = coArray<coFloat>(in->data, layer->GetNbInputs());
		coArray<coFloat> outputs = coArray<coFloat>(out->data, layer->GetNbOutputs());
		coComputeNeuralOutputs(*layer, inputs, outputs);
		coSwap(tempInputs, tempOutputs);
		in = &tempInputs;
		out = &tempOutputs;
	}

	for (coUint i = 0; i < _outputs.count; ++i)
	{
		_outputs[i] = (*in)[i];
	}
}
