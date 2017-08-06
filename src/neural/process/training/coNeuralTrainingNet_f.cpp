// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "neural/pch.h"
#include "neural/process/training/coNeuralTrainingNet_f.h"
#include "neural/process/training/coNeuralTrainingNet.h"
#include "neural/process/training/coNeuralTrainingLayer.h"
#include "neural/process/training/coNeuralTrainingLayer_f.h"
#include "neural/process/training/coNeuralDataSet.h"
#include "neural/process/training/coNeuralTrainingConfig.h"
#include "neural/process/coNeuralLayer_f.h"
#include "neural/process/coNeuralNet_f.h"
#include "neural/process/coNeuralActivation_f.h"
#include "neural/network/coNeuralNet.h"
#include "neural/network/coNeuralLayer.h"
#include "lang/result/coResult_f.h"

void coComputeForwardPass(coNeuralTrainingNet& _trainingNet, const coArray<coFloat>& _targetInputs)
{
	coArray<coFloat> inputs = _targetInputs;
	for (coNeuralTrainingLayer* trainingLayer : _trainingNet.GetTrainingLayers())
	{
		coComputeOutputs(*trainingLayer->layer, inputs, trainingLayer->outputs);
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
			gradients[i] = error * coComputeNeuralActivationDerivativeTransfer(value);
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
				sum += nextGradients.data[l] * nextWeights.data[weightIndex];
			}
			const coFloat output = curOutputs.data[k];
			curGradients.data[k] = sum * coComputeNeuralActivationDerivativeTransfer(output);
		}
	}
}

void coClearForEpoch(coNeuralTrainingNet& _trainingNet)
{
	for (coNeuralTrainingLayer* trainingLayer : _trainingNet.GetTrainingLayers())
	{
		coClearForEpoch(*trainingLayer);
	}
}

void coUpdateWeights(coNeuralTrainingNet& _trainingNet, coFloat _learningRate, coFloat _momentum, const coArray<coFloat>& _inputs)
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

		const coFloat decay = 0.0f;//0.000001f;

		coUint weightIndex = 0;
		for (coUint k = 0; k < nbOutputs; ++k)
		{
			const coFloat gradient = gradients.data[k];

			// Biases
			{
				coFloat& curBias = biasBuffer.data[k];
				coFloat& curDelta = biasDeltas.data[k];
				curDelta = _learningRate * gradient + _momentum * curDelta - decay * _learningRate * curBias;
				curBias += curDelta;
			}

			// Weights
			for (coUint l = 0; l < nbInputs; ++l)
			{
				coFloat& curWeight = weightBuffer.data[weightIndex];
				coFloat& curDelta = weightDeltas.data[weightIndex];
				curDelta = _learningRate * gradient * layerInputs.data[l] + _momentum * curDelta - decay * _learningRate* curWeight;
				curWeight += curDelta;
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

coResult coTrain(coNeuralTrainingNet& _trainingNet, const coNeuralDataSet& _dataSet, const coNeuralTrainingConfig& _config)
{
	// References:
	// - "On Large-Batch Training for Deep Learning: Generalization Gap and Sharp Minima" (arXiv:1609.04836).

	coTRY(_config.targetError >= 0.0f, nullptr);

	const coFloat meanSquareErrorTarget = coPow2(_config.targetError);

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

#ifdef coDEBUG
	coDynamicArray<coFloat> errors;
	coReserve(errors, _dataSet.nbSamples);
#endif

	// For each epoch
	coFloat err;
	coUint nbEpochs = 0;
	do
	{
		coClearForEpoch(_trainingNet);
		coShuffle(sampleIndices, seed);

		const coUint32 nbTrainingSamples = coUint32(_config.sampleRatio * _dataSet.nbSamples);
		const coUint32 nbValidationSamples = _dataSet.nbSamples - nbTrainingSamples;

		// For each sample
		for (coUint i = 0; i < nbTrainingSamples; ++i)
		{
			const coUint sampleIndex = sampleIndices[i];
			const coArray<coFloat> sampleInputs(_dataSet.inputs.data + sampleIndex * nbNetInputs, nbNetInputs);
			const coArray<coFloat> sampleOutputs(_dataSet.outputs.data + sampleIndex * nbNetOutputs, nbNetOutputs);
			coComputeForwardPass(_trainingNet, sampleInputs);
			coComputeGradients(_trainingNet, sampleOutputs);
			coUpdateWeights(_trainingNet, _config.learningRate, _config.momentum, sampleInputs);
		}

		// Error
		{
			err = 0.0f;
			for (coUint i = nbTrainingSamples; i < _dataSet.nbSamples; ++i)
			{
				const coUint sampleIndex = sampleIndices[i];
				const coArray<coFloat> sampleInputs(_dataSet.inputs.data + sampleIndex * nbNetInputs, nbNetInputs);
				const coArray<coFloat> sampleOutputs(_dataSet.outputs.data + sampleIndex * nbNetOutputs, nbNetOutputs);
				coComputeForwardPass(_trainingNet, sampleInputs);
				err += coComputeErrors(_trainingNet, sampleOutputs);
			}

			coASSERT(nbValidationSamples);
			err /= coFloat(nbValidationSamples);
			err *= 0.5f;

#ifdef coDEBUG
			coPushBack(errors, err);
#endif
		}

		++nbEpochs;
	} while (err > meanSquareErrorTarget && nbEpochs < _config.maxNbEpochs);

	return true;
}
