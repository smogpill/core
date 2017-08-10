// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "neural/pch.h"
#include "neural/process/training/coNeuralTrainingModel.h"
#include "neural/process/training/coNeuralTrainingLayer.h"
#include "neural/network/coNeuralModel.h"
#include "neural/network/coNeuralLayer.h"
#include "container/array/coDynamicArray_f.h"
#include "lang/result/coResult_f.h"

coNeuralTrainingModel::coNeuralTrainingModel(coNeuralModel& _net)
	: net(&_net)
{
}

coNeuralTrainingModel::~coNeuralTrainingModel()
{
	coDeleteElementsAndClear(trainingLayers);
}

coResult coNeuralTrainingModel::OnInit(const coObject::InitConfig& _config)
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
		coResize(trainingLayer->biasMs, nbOutputs);
		coFill(trainingLayer->biasMs, 0.0f);
		coResize(trainingLayer->weightMs, nbWeights);
		coFill(trainingLayer->weightMs, 0.0f);
		coResize(trainingLayer->biasVs, nbOutputs);
		coFill(trainingLayer->biasVs, 0.0f);
		coResize(trainingLayer->weightVs, nbWeights);
		coFill(trainingLayer->weightVs, 0.0f);
		coPushBack(trainingLayers, trainingLayer);
	}

	return true;
}
