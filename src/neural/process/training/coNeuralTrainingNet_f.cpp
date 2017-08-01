// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "neural/pch.h"
#include "neural/process/training/coNeuralTrainingNet_f.h"
#include "neural/process/training/coNeuralTrainingNet.h"
#include "neural/process/training/coNeuralTrainingLayer.h"
#include "neural/process/training/coNeuralDataSet.h"
#include "neural/process/coNeuralLayer_f.h"
#include "neural/process/coNeuralNet_f.h"
#include "neural/process/coNeuralSigmoid_f.h"
#include "neural/network/coNeuralNet.h"
#include "neural/network/coNeuralLayer.h"
#include "lang/result/coResult_f.h"

void coComputeForwardPass(coNeuralTrainingNet& _trainingNet, const coArray<coFloat>& _targetInputs)
{
	coArray<coFloat> inputs = _targetInputs;
	for (coNeuralTrainingLayer* trainingLayer : _trainingNet.GetTrainingLayers())
	{
		coNeuralLayer* layer = trainingLayer->layer;
		coComputeNeuralOutputs(*layer, inputs, trainingLayer->outputs);
		inputs = trainingLayer->outputs;
	}
}

void coComputeGradients(coNeuralTrainingNet& _trainingNet, const coArray<coFloat>& _targetOutputs)
{
	const coArray<coNeuralTrainingLayer*>& trainingLayers = _trainingNet.GetTrainingLayers();

	// Last layer
	{
		coNeuralTrainingLayer* trainingLayer = coBack(trainingLayers);
		const coUint nbOutputs = trainingLayer->outputs.count;
		coArray<coFloat>& gradients = trainingLayer->gradients;
		const coArray<coFloat>& values = trainingLayer->outputs;
		for (coUint i = 0; i < nbOutputs; ++i)
		{
			const coFloat value = values[i];
			const coFloat targetValue = _targetOutputs[i];
			const coFloat error = targetValue - value;
			gradients[i] = error * coComputeNeuralSigmoidDerivative(value);
		}
	}

	// Rest of the layers
	for (coInt j = trainingLayers.count - 2; j >= 0; --j)
	{
		coNeuralTrainingLayer* curTrainingLayer = trainingLayers[j];
		coNeuralTrainingLayer* nextTrainingLayer = trainingLayers[j + 1];
		coArray<coFloat>& curGradients = curTrainingLayer->gradients;
		const coArray<coFloat>& nextGradients = nextTrainingLayer->gradients;
		const coArray<coFloat>& curOutputs = curTrainingLayer->outputs;

		const coNeuralLayer* curData = curTrainingLayer->layer;
		const coNeuralLayer* nextData = nextTrainingLayer->layer;
		const coArray<coFloat>& nextWeights = nextData->GetWeightBuffer();
		const coUint nbCurOutputs = curData->GetNbOutputs();
		const coUint nbNextOutputs = nextData->GetNbOutputs();
		for (coUint k = 0; k < nbCurOutputs; ++k)
		{
			coFloat sum = 0.0f;
			for (coUint l = 0; l < nbNextOutputs; ++l)
			{
				const coUint weightIndex = l * nbCurOutputs + k;
				sum += nextGradients[l] * nextWeights[weightIndex];
			}
			const coFloat output = curOutputs[k];
			curGradients[k] = sum * coComputeNeuralSigmoidDerivative(output);
		}
	}
}

void coUpdateWeights(coNeuralTrainingNet& _trainingNet, coFloat _learnRate, coFloat _momentum, const coArray<coFloat>& _inputs)
{
	const coArray<coNeuralTrainingLayer*>& trainingLayers = _trainingNet.GetTrainingLayers();
	coArray<coFloat> layerInputs(_inputs);
	for (coNeuralTrainingLayer* trainingLayer : trainingLayers)
	{
		coNeuralLayer* data = trainingLayer->layer;
		const coUint nbInputs = data->GetNbInputs();
		const coUint nbOutputs = data->GetNbOutputs();
		coArray<coFloat>& biasBuffer = data->GetBiasBuffer();
		coArray<coFloat>& weightBuffer = data->GetWeightBuffer();
		coArray<coFloat>& biasDeltas = trainingLayer->biasDeltas;
		coArray<coFloat>& weightDeltas = trainingLayer->weightDeltas;
		const coArray<coFloat>& gradients = trainingLayer->gradients;
		coUint weightIndex = 0;
		for (coUint k = 0; k < nbOutputs; ++k)
		{
			const coFloat gradient = gradients[k];

			// Biases
			{
				coFloat& curDelta = biasDeltas[k];
				const coFloat prevDelta = curDelta;
				curDelta = _learnRate * gradient;
				biasBuffer[k] += curDelta + _momentum * prevDelta;
			}

			// Weights
			for (coUint l = 0; l < nbInputs; ++l)
			{
				coFloat& curDelta = weightDeltas[weightIndex];
				const coFloat prevDelta = curDelta;
				curDelta = _learnRate * gradient * layerInputs[l];
				weightBuffer[weightIndex] += curDelta + _momentum * prevDelta;
				++weightIndex;
			}
		}

		layerInputs = trainingLayer->outputs;
	}
}

coFloat coComputeErrors(const coNeuralTrainingNet& _trainingNet, const coArray<coFloat>& _targetOutputs)
{
	coFloat error = 0.0f;
	const coArray<coNeuralTrainingLayer*>& trainingLayers = _trainingNet.GetTrainingLayers();
	coNeuralTrainingLayer* lastTrainingLayer = coBack(trainingLayers);
	const coArray<coFloat>& outputs = lastTrainingLayer->outputs;
	coASSERT(outputs.count == _targetOutputs.count);
	const coUint nbOutputs = lastTrainingLayer->outputs.count;
	for (coUint i = 0; i < nbOutputs; ++i)
	{
		error += coPow2(_targetOutputs[i] - outputs[i]);
	}
	return error;
}

coResult coTrain(coNeuralTrainingNet& _trainingNet, const coNeuralDataSet& _dataSet, coFloat _targetError, coUint _nbMaxEpochs)
{
	// References:
	// - "On Large-Batch Training for Deep Learning: Generalization Gap and Sharp Minima" (arXiv:1609.04836).

	coTRY(_targetError >= 0.0f, nullptr);

	const coFloat learnRate = 0.4f;
	const coFloat momentum = 0.9f;

	//coTRY(learnRate < 1.0f, nullptr);

	coNeuralNet* net = _trainingNet.GetNet();
	coTRY(net, nullptr);

	const coUint nbNetInputs = net->GetNbInputs();
	const coUint nbNetOutputs = net->GetNbOutputs();

	coDynamicArray<coUint> sampleIndices;
	coResize(sampleIndices, _dataSet.nbSamples);
	for (coUint i = 0; i < sampleIndices.count; ++i)
		sampleIndices[i] = i;

	coUint32 seed = 77777777;

	// For each epoch
	coFloat err;
	coUint nbEpochs = 0;
	do
	{
		err = 0.0f;

		coShuffle(sampleIndices, seed);

		// For each sample
		for (coUint i = 0; i < _dataSet.nbSamples; ++i)
		{
			const coUint sampleIndex = sampleIndices[i];
			const coArray<coFloat> sampleInputs(_dataSet.inputs.data + sampleIndex * nbNetInputs, nbNetInputs);
			const coArray<coFloat> sampleOutputs(_dataSet.outputs.data + sampleIndex * nbNetOutputs, nbNetOutputs);
			coComputeForwardPass(_trainingNet, sampleInputs);
			coComputeGradients(_trainingNet, sampleOutputs);
			coUpdateWeights(_trainingNet, learnRate, momentum, sampleInputs);
			err += coComputeErrors(_trainingNet, sampleOutputs);
		}

		coASSERT(_dataSet.nbSamples);
		err /= coFloat(_dataSet.nbSamples);
		err *= 0.5f;

		++nbEpochs;
	} while (/*err > _targetError && */nbEpochs < _nbMaxEpochs);

	return true;
}
