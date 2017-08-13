// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "neural/pch.h"
#include "neural/process/coNeuralNet_f.h"
#include "neural/process/coNeuralLayer_f.h"
#include "neural/network/coNeuralModel.h"
#include "neural/network/coNeuralLayer.h"
#include "math/scalar/coFloat_f.h"

void coResetWeightsAndBiases(coNeuralModel& _this, coUint32& _seed)
{
	for (coNeuralLayer* layer : _this.GetLayers())
	{
		coResetWeightsAndBiases(*layer, _seed);
	}
}

void coComputeOutputs(const coNeuralModel& _this, const coArray<coFloat>& _inputs, coArray<coFloat>& _outputs)
{
	const coArray<coNeuralLayer*> layers = _this.GetLayers();
	coASSERT(layers.count > 0);
	coASSERT(_this.GetNbInputs() == _inputs.count);
	coASSERT(_this.GetNbOutputs() == _outputs.count);

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
		coComputeOutputs(*layer, inputs, outputs);
		coSwap(tempInputs, tempOutputs);
		in = &tempInputs;
		out = &tempOutputs;
	}

	for (coUint i = 0; i < _outputs.count; ++i)
	{
		_outputs[i] = (*in)[i];
	}
}

void coComputeOutputSet(const coNeuralModel& _this, const coArray<coFloat>& _inputSet, coArray<coFloat>& _outputSet, coUint _nbElements)
{
	const coArray<coNeuralLayer*> layers = _this.GetLayers();
	coASSERT(layers.count > 0);
	const coUint nbInputs = _this.GetNbInputs();
	const coUint nbOutputs = _this.GetNbOutputs();
	coASSERT(_inputSet.count == nbInputs * _nbElements);
	coASSERT(_outputSet.count == nbOutputs * _nbElements);

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

	for (coUint i = 0; i < _nbElements; ++i)
	{
		coArray<coFloat> in(_inputSet.data + i * nbInputs, nbInputs);
		coArray<coFloat> out = tempOutputs;

		for (const coNeuralLayer* layer : layers)
		{
			coArray<coFloat> inArray(in.data, layer->GetNbInputs());
			coArray<coFloat> outArray(out.data, layer->GetNbOutputs());
			coComputeOutputs(*layer, inArray, outArray);
			coSwap(tempInputs, tempOutputs);
			in = tempInputs;
			out = tempOutputs;
		}

		coArray<coFloat> outputs(_outputSet.data + i * nbOutputs, nbOutputs);

		for (coUint j = 0; j < outputs.count; ++j)
		{
			outputs[j] = in[j];
		}
	}
}
