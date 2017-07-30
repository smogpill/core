// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "neural/pch.h"
#include "neural/process/training/coNeuralTrainingNet.h"
#include "neural/process/training/coNeuralTrainingLayer.h"
#include "neural/network/coNeuralNet.h"
#include "neural/network/coNeuralLayer.h"
#include "container/array/coDynamicArray_f.h"
#include "lang/result/coResult_f.h"

coNeuralTrainingNet::coNeuralTrainingNet(const coNeuralNet& _net)
	: net(&_net)
{

}

coNeuralTrainingNet::~coNeuralTrainingNet()
{
	for (coNeuralTrainingLayer* layer : trainingLayers)
		delete layer;
}

coResult coNeuralTrainingNet::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	coTRY(net, nullptr);
	const coArray<coNeuralLayer*>& layers = net->GetLayers();
	coReserve(trainingLayers, layers.count);
	coArray<coFloat> inputs;
	for (coNeuralLayer* layer : layers)
	{
		coNeuralTrainingLayer* trainingLayer = new coNeuralTrainingLayer();
		trainingLayer->layer = layer;
		const coUint nbInputs = layer->GetNbInputs();
		const coUint nbOutputs = layer->GetNbOutputs();
		coResize(trainingLayer->weightDeltas, nbInputs * nbOutputs);
		coFill(trainingLayer->weightDeltas, 0.0f);
		coResize(trainingLayer->biasDeltas, nbOutputs);
		coFill(trainingLayer->biasDeltas, 0.0f);
		coResize(trainingLayer->outputs, nbOutputs);
		coResize(trainingLayer->gradients, nbOutputs);
		coPushBack(trainingLayers, trainingLayer);
		trainingLayer->inputs = inputs;
		inputs = trainingLayer->outputs;
	}

	return true;
}
