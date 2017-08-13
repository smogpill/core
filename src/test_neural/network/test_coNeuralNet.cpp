// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_neural/pch.h"
#include "test/unit/coTest.h"
#include "neural/network/coNeuralModel.h"
#include "neural/network/coNeuralLayer.h"
#include "neural/process/coNeuralNet_f.h"
#include "neural/process/training/coNeuralDataSet.h"
#include "neural/process/training/coNeuralTrainingModel.h"
#include "neural/process/training/coNeuralTrainingModel_f.h"
#include "neural/process/training/coNeuralTrainingConfig.h"
#include "neural/process/compute/coNeuralComputeOutputs.h"
#include "neural/process/tensor/coNeuralNormalizationConfig.h"
#include "neural/process/tensor/coNeuralTensor_f.h"
#include "math/scalar/coFloat_f.h"
#include "pattern/scope/coDefer.h"

coTEST(coNeuralModel, Init)
{
	coNeuralLayer data(1, 1);
	coEXPECT(data.Init(coNeuralLayer::InitConfig()));
	coArray<coNeuralLayer*> layerDatas = { &data };
	coNeuralModel net(layerDatas);
	coEXPECT(net.Init(coNeuralModel::InitConfig()));
}

coTEST(coNeuralModel, TrainALine)
{
	// Neural network
	const coUint nbHiddenLayers = 2;
	const coUint nbHiddenNeurons = 8;
	const coFloat desiredError = 0.01f;

	const coFloat xMin = -10.0f;
	const coFloat xMax = 10.0f;
	const coFloat xRange = xMax - xMin;

	// Function
	coNeuralNormalizationConfig inputNormalizationConfig;
	inputNormalizationConfig.means = { (xMin + xMax) * 0.5f };
	inputNormalizationConfig.deviations = { xRange * 0.5f };
	coNeuralNormalizationConfig outputNormalizationConfig;
	outputNormalizationConfig.means = { 0.0f };
	outputNormalizationConfig.deviations = { 1.0f };

	auto ComputeValue = [&](coFloat _x)
	{
		return 0.5f * coSin(_x);
	};
	auto ComputeSet = [&](coArray<coFloat>& _outputs, const coArray<coFloat>& _inputs)
	{
		coASSERT(_outputs.count == _inputs.count);
		for (coUint i = 0; i < _inputs.count; ++i)
		{
			_outputs[i] = ComputeValue(_inputs[i]);
		}
	};

	coDynamicArray<coNeuralLayer*> layerDatas;
	coDEFER() { coDeleteElementsAndClear(layerDatas); };
	
	for (coUint i = 0; i < nbHiddenLayers; ++i)
	{
		coNeuralLayer* hiddenLayer = new coNeuralLayer(i ? nbHiddenNeurons : 1, nbHiddenNeurons);
		coEXPECT(hiddenLayer->Init(coNeuralLayer::InitConfig()));
		coPushBack(layerDatas, hiddenLayer);
	}

	coNeuralLayer* outputLayer = new coNeuralLayer(nbHiddenNeurons, 1);
	coEXPECT(outputLayer->Init(coNeuralLayer::InitConfig()));
	coPushBack(layerDatas, outputLayer);

	coNeuralModel model(layerDatas);
	coEXPECT(model.Init(coNeuralModel::InitConfig()));

	coUint32 seed = 777777777;

	// Train
	coUint32 nbEpochs = 0;
	{
		const coUint nbSamples = 100000;
		coDynamicArray<coFloat> inputs;
		coDynamicArray<coFloat> outputs;
		coResize(inputs, nbSamples);
		coResize(outputs, nbSamples);
		{
			for (coUint i = 0; i < nbSamples; ++i)
			{
				inputs[i] = xMin + coRand01(seed) * xRange;
			}
			ComputeSet(outputs, inputs);
			coNormalizeSet(inputs, nbSamples, inputNormalizationConfig);
			coNormalizeSet(outputs, nbSamples, outputNormalizationConfig);
		}

		coNeuralDataSet dataSet;
		{
			dataSet.inputs.values = inputs;
			dataSet.outputs.values = outputs;
		}

		coNeuralTrainingModel trainingModel(model);
		coEXPECT(trainingModel.Init(coObject::InitConfig()));

		coResetWeightsAndBiases(model, seed);

		coNeuralTrainingConfig config;
		config.targetError = desiredError;
		config.maxNbEpochs = 100;
		config.momentum = 0.0f; // temp
		config.nbEpochsPerValidation = 1;
		coEXPECT(coTrain(trainingModel, dataSet, config, seed, nbEpochs));
	}

	// Tests
	{
		const coUint nbSamples = 1000;

		coDynamicArray<coFloat> inputs;
		coDynamicArray<coFloat> outputs;
		coDynamicArray<coFloat> expectedOutputs;
		coResize(inputs, nbSamples);
		coResize(outputs, nbSamples);
		coResize(expectedOutputs, nbSamples);
		
		for (coUint i = 0; i < nbSamples; ++i)
		{
			inputs[i] = xMin + xRange * coFloat(i) / (nbSamples - 1);
		}
		ComputeSet(expectedOutputs, inputs);
		coNormalizeSet(inputs, nbSamples, inputNormalizationConfig);
		coNormalizeSet(expectedOutputs, nbSamples, outputNormalizationConfig);
		coComputeOutputSet(model, inputs, outputs, nbSamples);
		const coFloat error = coComputeMeanSquaredError(outputs, expectedOutputs);
		const coFloat expectedMeanSquareError = desiredError * desiredError;
		coEXPECT(error <= expectedMeanSquareError);
	}
}
