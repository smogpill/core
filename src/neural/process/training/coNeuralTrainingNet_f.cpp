// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "neural/pch.h"
#include "neural/process/training/coNeuralTrainingNet_f.h"
#include "neural/process/training/coNeuralTrainingNet.h"
#include "neural/process/coNeuralLayer_f.h"
#include "neural/process/training/coNeuralTrainingLayer.h"

void coComputeForwardPass(coNeuralTrainingNet& _net, const coArray<coFloat>& _inputs)
{
	coArray<coFloat> inputs = _inputs;
	for (coNeuralTrainingLayer* trainingLayer : _net.GetTrainingLayers())
	{
		coNeuralLayer* layer = trainingLayer->layer;
		coComputeNeuralOutputs(*layer, inputs, trainingLayer->outputs);
		inputs = trainingLayer->outputs;
	}
}
