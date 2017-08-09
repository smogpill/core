// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "neural/pch.h"
#include "neural/process/training/coNeuralTrainingNet.h"
#include "neural/process/training/coNeuralTrainingLayer.h"
#include "neural/network/coNeuralNet.h"
#include "neural/network/coNeuralLayer.h"
#include "container/array/coDynamicArray_f.h"
#include "lang/result/coResult_f.h"

coNeuralTrainingNet::coNeuralTrainingNet(coNeuralNet& _net)
	: net(&_net)
{
}

coNeuralTrainingNet::~coNeuralTrainingNet()
{
	coDeleteElementsAndClear(trainingLayers);
}

coResult coNeuralTrainingNet::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	coTRY(net, nullptr);
	const coArray<coNeuralLayer*>& layers = net->GetLayers();
	coReserve(trainingLayers, layers.count);
	for (coNeuralLayer* layer : layers)
	{
		coNeuralTrainingLayer* trainingLayer = new coNeuralTrainingLayer();
		trainingLayer->layer = layer;
		const coUint nbInputs = layer->GetNbInputs();
		const coUint nbOutputs = layer->GetNbOutputs();
		const coUint nbWeights = nbInputs * nbOutputs;
		coResize(trainingLayer->weightDeltas, nbWeights);
		coFill(trainingLayer->weightDeltas, 0.0f);
		coResize(trainingLayer->biasDeltas, nbOutputs);
		coFill(trainingLayer->biasDeltas, 0.0f);
		coResize(trainingLayer->outputs, nbOutputs);
		coFill(trainingLayer->outputs, 0.0f);
		coResize(trainingLayer->deltas, nbOutputs);
		coFill(trainingLayer->deltas, 0.0f);
		coResize(trainingLayer->biasAccs, nbOutputs);
		coFill(trainingLayer->biasAccs, 0.0f);
		coResize(trainingLayer->weightAccs, nbWeights);
		coFill(trainingLayer->weightAccs, 0.0f);
		coResize(trainingLayer->biasRunningAverages, nbOutputs);
		coFill(trainingLayer->biasRunningAverages, 0.0f);
		coResize(trainingLayer->weightRunningAverages, nbWeights);
		coFill(trainingLayer->weightRunningAverages, 0.0f);
		coPushBack(trainingLayers, trainingLayer);
	}

	return true;
}
