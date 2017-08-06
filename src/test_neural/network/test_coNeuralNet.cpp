// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_neural/pch.h"
#include "test/unit/coTest.h"
#include "neural/network/coNeuralNet.h"
#include "neural/network/coNeuralLayer.h"
#include "neural/process/coNeuralNet_f.h"
#include "neural/process/training/coNeuralDataSet.h"
#include "neural/process/training/coNeuralTrainingNet.h"
#include "neural/process/training/coNeuralTrainingNet_f.h"
#include "neural/process/training/coNeuralTrainingConfig.h"
#include "neural/process/compute/coNeuralComputeOutputs.h"
#include "math/scalar/coFloat_f.h"
#include "pattern/scope/coDefer.h"

coTEST(coNeuralNet, Init)
{
	coNeuralLayer data(1, 1);
	coEXPECT(data.Init(coNeuralLayer::InitConfig()));
	coArray<coNeuralLayer*> layerDatas = { &data };
	coNeuralNet net(layerDatas);
	coEXPECT(net.Init(coNeuralNet::InitConfig()));
}

coTEST(coNeuralNet, TrainALine)
{
	// Neural network
	const coUint nbHiddenLayers = 2;
	const coUint nbHiddenNeurons = 8;
	const coFloat desiredError = 0.01f;

	// Function
	const coFloat xMin = -10.0f;
	const coFloat xMax = 10.0f;
	const coFloat xRange = xMax - xMin;
	const coFloat yMin = -1.0f;
	const coFloat yMax = 1.0f;
	const coFloat yRange = yMax - yMin;
	auto NormalizeX = [&](coFloat _x)
	{
		return coClamp01((_x - xMin) / xRange);// *2.0f - 1.0f;
	};
	auto NormalizeY = [&](coFloat _y)
	{
		return coClamp01((_y - yMin) / yRange);// *2.0f - 1.0f;
	};
	auto ComputeValue = [&](coFloat _x)
	{
		return 0.5f * coSin(_x);
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

	coNeuralNet net(layerDatas);
	coEXPECT(net.Init(coNeuralNet::InitConfig()));

	coUint32 seed = 777777777;

	// Train
	{
		const coUint nbSamples = 100000;
		coDynamicArray<coFloat> inputs;
		coDynamicArray<coFloat> outputs;
		{
			coResize(inputs, nbSamples);
			coResize(outputs, nbSamples);
			for (coUint i = 0; i < nbSamples; ++i)
			{
				const coFloat x = xMin + coRand01(seed) * xRange;
				const coFloat y = ComputeValue(x);
				inputs[i] = NormalizeX(x);
				outputs[i] = NormalizeY(y);
			}
		}

		coNeuralDataSet dataSet;
		{
			dataSet.nbSamples = nbSamples;
			dataSet.inputs = inputs;
			dataSet.outputs = outputs;
		}

		coNeuralTrainingNet trainingNet(net);
		coEXPECT(trainingNet.Init(coObject::InitConfig()));

		coResetWeightsAndBiases(net, seed);

		coNeuralTrainingConfig config;
		config.targetError = desiredError;
		config.maxNbEpochs = 250;
		coEXPECT(coTrain(trainingNet, dataSet, config));
	}

	// Checks
	{
		const coUint nbChecks = 1000;

		coDynamicArray<coFloat> allExpectedOutputs; // can be plotted 
		coDynamicArray<coFloat> allOutputs; // can be plotted 
		coDynamicArray<coFloat> inputs;
		coDynamicArray<coFloat> outputs;
		coResize(allExpectedOutputs, nbChecks);
		coResize(allOutputs, nbChecks);
		coResize(inputs, 1);
		coResize(outputs, 1);
		
		coFloat error = 0.0f;
		for (coUint i = 0; i < nbChecks; ++i)
		{
			const coFloat x = xMin + xRange * coFloat(i) / (nbChecks - 1);
			const coFloat y = ComputeValue(x);

			const coFloat input = NormalizeX(x);
			const coFloat expectedOutput = NormalizeY(y);

			coFloat output;
			{
				inputs[0] = input;
				coComputeOutputs(net, inputs, outputs);
				output = outputs[0];
			}

			allOutputs[i] = output;
			allExpectedOutputs[i] = expectedOutput;

			error += coPow2(expectedOutput - output);
		}
		error *= 0.5f / nbChecks;

		const coFloat expectedMeanSquareError = desiredError * desiredError;
		coEXPECT(error <= expectedMeanSquareError);
	}
}
