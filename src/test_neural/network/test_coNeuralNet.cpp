// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_neural/pch.h"
#include "test/unit/coTest.h"
#include "neural/network/coNeuralNet.h"
#include "neural/network/coNeuralLayerData.h"
#include "math/scalar/coFloat_f.h"

coTEST(coNeuralNet, Init)
{
	coNeuralLayerData data(1, 1);
	coEXPECT(data.Init(coNeuralLayerData::InitConfig()));
	coArray<coNeuralLayerData*> layerDatas = { &data };
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
	const coFloat error = 0.01f;
	coNeuralLayerData data0(1, 8);
	coNeuralLayerData data1(8, 1);
	coEXPECT(data0.Init(coNeuralLayerData::InitConfig()));
	coEXPECT(data1.Init(coNeuralLayerData::InitConfig()));
	coArray<coNeuralLayerData*> layerDatas = { &data0, &data1 };
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
		const coUint nbSamples = 10000;
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

		coNeuralNet::DataSet dataSet;
		{
			dataSet.nbSamples = nbSamples;
			dataSet.inputs = inputs;
			dataSet.outputs = outputs;
		}

		const coUint nbMaxEpochs = 1000;
		coEXPECT(net.Train(dataSet, error, nbMaxEpochs));
	}

	// Checks
	{
		coDynamicArray<coFloat> outputs;
		coResize(outputs, 1);

		coUint nbChecks = 100;
		for (coUint i = 0; i < nbChecks; ++i)
		{
			const coFloat x = GetRandomX();
			const coFloat input = ConvertXToNet(x);
			coEXPECT(net.Compute({ input }, outputs));
			const coFloat value = ConvertYFromNet(outputs[0]);
			const coFloat expectedValue = a * x + b;
			coEXPECT(coNearEqual(value, expectedValue, error));
		}
	}
}
