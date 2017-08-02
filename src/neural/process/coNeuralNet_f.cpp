// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "neural/pch.h"
#include "neural/process/coNeuralNet_f.h"
#include "neural/process/coNeuralLayer_f.h"
#include "neural/network/coNeuralNet.h"
#include "neural/network/coNeuralLayer.h"
#include "math/scalar/coFloat_f.h"

void coResetWeightsAndBiases(coNeuralNet& _this, coUint32& _seed)
{
	for (coNeuralLayer* layer : _this.GetLayers())
	{
		coResetWeightsAndBiases(*layer, _seed);
	}
}

void coComputeNeuralOutputs(const coNeuralNet& _this, const coArray<coFloat>& _inputs, coArray<coFloat>& _outputs)
{
	const coArray<coNeuralLayer*> layers = _this.GetLayers();
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
