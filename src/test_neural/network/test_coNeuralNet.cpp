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
#include "neural/process/compute/coNeuralComputeOutputs.h"
#include "math/scalar/coFloat_f.h"

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
	const coFloat xMin = -20.0f;
	const coFloat xMax = 20.0f;
	const coFloat xRange = xMax - xMin;
	const coFloat yMin = -1.0f;
	const coFloat yMax = 1.0f;
	const coFloat yRange = yMax - yMin;

	const coUint nbHiddenNeurons = 16;
	//coASSERT(yRange > 0.0f);
	const coFloat desiredError = 0.01f;
	coNeuralLayer data0(1, nbHiddenNeurons);
	coNeuralLayer data1(nbHiddenNeurons, 1);
	coEXPECT(data0.Init(coNeuralLayer::InitConfig()));
	coEXPECT(data1.Init(coNeuralLayer::InitConfig()));
	coDynamicArray<coNeuralLayer*> layerDatas;
	coPushBack(layerDatas, &data0);
	coPushBack(layerDatas, &data1);
	coNeuralNet net(layerDatas);
	coEXPECT(net.Init(coNeuralNet::InitConfig()));

	coUint32 seed = 777777777;

	auto ConvertXToNet = [&](coFloat _x)
	{
		return coClamp01((_x - xMin) / xRange);
	};
	auto ConvertYToNet = [&](coFloat _y)
	{
		return coClamp01((_y - yMin) / yRange);
	};
	auto ConvertXFromNet = [&](coFloat _x)
	{
		return xMin + _x * xRange;
	};
	auto ConvertYFromNet = [&](coFloat _y)
	{
		return yMin + _y * yRange;
	};
	auto ComputeValue = [&](coFloat _x)
	{
		return coSin(_x);
	};

	// Train
	{
		const coUint nbSamples = 1000;
		coDynamicArray<coFloat> inputs;
		coDynamicArray<coFloat> outputs;
		{
			coResize(inputs, nbSamples);
			coResize(outputs, nbSamples);
			for (coUint i = 0; i < nbSamples; ++i)
			{
				const coFloat x = xMin + coRand01(seed) * xRange;
				const coFloat y = ComputeValue(x);
				inputs[i] = ConvertXToNet(x);
				outputs[i] = ConvertYToNet(y);
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

		const coUint nbMaxEpochs = 1000;
		coEXPECT(coTrain(trainingNet, dataSet, desiredError, nbMaxEpochs));
	}

	// Checks
	{
		const coUint nbChecks = 1000;

		coDynamicArray<coFloat> allOutputs; // can be plotted 
		coResize(allOutputs, nbChecks);
		coDynamicArray<coFloat> inputs;
		coDynamicArray<coFloat> outputs;
		coResize(inputs, 1);
		coResize(outputs, 1);
		
		coFloat error = 0.0f;
		for (coUint i = 0; i < nbChecks; ++i)
		{
			const coFloat x = xMin + xRange * coFloat(i) / (nbChecks - 1);
			const coFloat y = ComputeValue(x);

			const coFloat input = ConvertXToNet(x);
			const coFloat expectedOutput = ConvertYToNet(y);

			coFloat output;
			{
				inputs[0] = input;
				coComputeNeuralOutputs(net, inputs, outputs);
				output = outputs[0];
			}

			allOutputs[i] = output;
			error += coPow2(expectedOutput - output);
		}
		error *= 0.5f / nbChecks;

		coEXPECT(error <= desiredError);
	}
}
