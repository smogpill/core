// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "neural/pch.h"
#include "neural/network/coNeuralModel.h"
#include "neural/network/coNeuralLayer.h"
#include "neural/process/coNeuralNet_f.h"
#include "neural/process/coNeuralLayer_f.h"
#include "neural/process/training/coNeuralTrainingLayer.h"
#include "neural/process/training/coNeuralTrainingModel.h"
#include "neural/process/training/coNeuralDataSet.h"
#include "neural/process/training/coNeuralTrainingModel_f.h"
#include "neural/process/compute/coNeuralComputeOutputs.h"
#include "neural/process/coNeuralActivation_f.h"
#include "lang/result/coResult_f.h"
#include "math/scalar/coFloat_f.h"
#include "math/scalar/coUint32_f.h"

coNeuralModel::coNeuralModel(const coArray<coNeuralLayer*> _layers)
	: layers(_layers)
{
}

coResult coNeuralModel::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	coTRY(layers.count, nullptr);
	return true;
}

coUint coNeuralModel::GetNbInputs() const
{
	if (layers.count)
	{
		const coNeuralLayer* firstLayer = layers[0];
		coASSERT(firstLayer);
		return firstLayer->GetNbInputs();
	}
	else
	{
		return 0;
	}
}

coUint coNeuralModel::GetNbOutputs() const
{
	if (layers.count)
	{
		const coNeuralLayer* firstLayer = coBack(layers);
		coASSERT(firstLayer);
		return firstLayer->GetNbOutputs();
	}
	else
	{
		return 0;
	}
}
