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
	const coFloat a = 7.0f;
	const coFloat b = 17.0f;
	const coFloat xMin = -100.0f;
	const coFloat xMax = 100.0f;
	const coFloat xRange = xMax - xMin;
	const coFloat yMin = a * xMin + b;
	const coFloat yMax = a * xMax + b;
	const coFloat yRange = yMax - yMin;
	//coASSERT(yRange > 0.0f);
	const coFloat desiredError = 0.01f;
	coNeuralLayer data0(1, 8);
	coNeuralLayer data1(8, 1);
	coEXPECT(data0.Init(coNeuralLayer::InitConfig()));
	coEXPECT(data1.Init(coNeuralLayer::InitConfig()));
	coDynamicArray<coNeuralLayer*> layerDatas;
	coPushBack(layerDatas, &data0);
	coPushBack(layerDatas, &data1);
	coNeuralNet net(layerDatas);
	coEXPECT(net.Init(coNeuralNet::InitConfig()));

	coUint32 seed = 777777777;

	auto GetRandomX = [&]()
	{
		return coRand01(seed) * xRange + xMin;
	};
	auto ConvertXToNet = [&](coFloat _x)
	{
		return coClamp01((_x - xMin) / xRange);
	};
	auto ConvertXFromNet = [&](coFloat _x)
	{
		return _x * xRange + xMin;
	};
	auto ConvertYToNet = [&](coFloat _y)
	{
		return coClamp01((_y - yMin) / yRange);
	};
	auto ConvertYFromNet = [&](coFloat _y)
	{
		return _y * yRange + yMin;
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
				const coFloat x = GetRandomX();
				inputs[i] = ConvertXToNet(x);
				outputs[i] = ConvertYToNet(a * x + b);
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

		coRandomizeWeightsAndBiases(net, seed);

		const coUint nbMaxEpochs = 1000;
		coEXPECT(coTrain(trainingNet, dataSet, desiredError, nbMaxEpochs));
	}

	// Checks
	{
		coDynamicArray<coFloat> outputs;
		coResize(outputs, 1);

		coFloat error = 0.0f;
		coUint nbChecks = 1000;
		for (coUint i = 0; i < nbChecks; ++i)
		{
			const coFloat x = GetRandomX();
			const coFloat input = ConvertXToNet(x);
			coComputeNeuralOutputs(net, { input }, outputs);
			const coFloat output = outputs[0];
			const coFloat expectedOutput = ConvertYToNet(a * x + b);
			error += coPow2(expectedOutput - output);
		}
		error *= 0.5f / nbChecks;

		coEXPECT(error <= desiredError);
	}
}
