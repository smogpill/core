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

void coComputeBackwardPass(coNeuralTrainingNet& _trainingNet, const coArray<coFloat>& _targetInputs, const coArray<coFloat>& _targetOutputs)
{
	const coArray<coNeuralTrainingLayer*>& trainingLayers = _trainingNet.GetTrainingLayers();
	
	// Compute deltas
	{
		// Output layer
		coNeuralTrainingLayer* outputTrainingLayer = coBack(trainingLayers);
		{
			const coUint nbOutputs = outputTrainingLayer->outputs.count;
			coArray<coFloat>& deltas = outputTrainingLayer->deltas;
			const coArray<coFloat>& outputs = outputTrainingLayer->outputs;
			for (coUint o = 0; o < nbOutputs; ++o)
			{
				const coFloat output = outputs.data[o];
				const coFloat targetOutput = _targetOutputs.data[o];
				const coFloat error = targetOutput - output;
				deltas.data[o] = error * coComputeNeuralActivationDerivativeTransfer(output);
			}
		}

		// Hidden layers
		coNeuralTrainingLayer* nextTrainingLayer = outputTrainingLayer;
		for (coInt l = trainingLayers.count - 2; l >= 0; --l)
		{
			coNeuralTrainingLayer* curTrainingLayer = trainingLayers[l];
			coArray<coFloat>& curDeltas = curTrainingLayer->deltas;
			const coArray<coFloat>& nextDeltas = nextTrainingLayer->deltas;
			const coArray<coFloat>& curOutputs = curTrainingLayer->outputs;

			const coNeuralLayer* curLayer = curTrainingLayer->layer;
			const coNeuralLayer* nextLayer = nextTrainingLayer->layer;
			const coArray<coFloat>& nextWeights = nextLayer->GetWeightBuffer();
			const coUint nbCurOutputs = curLayer->GetNbOutputs();
			const coUint nbNextOutputs = nextLayer->GetNbOutputs();
			for (coUint c = 0; c < nbCurOutputs; ++c)
			{
				coFloat sum = 0.0f;
				for (coUint n = 0; n < nbNextOutputs; ++n)
				{
					const coUint weightIndex = n * nbCurOutputs + c;
					sum += nextDeltas.data[n] * nextWeights.data[weightIndex];
				}
				const coFloat output = curOutputs.data[c];
				curDeltas.data[c] = sum * coComputeNeuralActivationDerivativeTransfer(output);
			}
			nextTrainingLayer = curTrainingLayer;
		}
	}

	// Acc
	coArray<coFloat> inputs = _targetInputs;
	for (coNeuralTrainingLayer* trainingLayer : trainingLayers)
	{
		coArray<coFloat>& biasAccs = trainingLayer->biasAccs;
		coArray<coFloat>& weightAccs = trainingLayer->weightAccs;
		const coArray<coFloat>& deltas = trainingLayer->deltas;
		const coArray<coFloat>& outputs = trainingLayer->outputs;
		const coUint nbInputs = inputs.count;
		const coUint nbOutputs = outputs.count;

		coUint weightIndex = 0;
		for (coUint o = 0; o < nbOutputs; ++o)
		{
			const coFloat d = deltas.data[o];
			biasAccs.data[o] += d;
			for (coUint i = 0; i < nbInputs; ++i)
			{
				weightAccs[weightIndex] += d * inputs.data[i];
				++weightIndex;
			}
		}
		inputs = outputs;
	}
}

void coClearForEpoch(coNeuralTrainingNet& _trainingNet)
{
	for (coNeuralTrainingLayer* trainingLayer : _trainingNet.GetTrainingLayers())
	{
		coClearForEpoch(*trainingLayer);
	}
}

void coUpdateWeights(coNeuralTrainingNet& _trainingNet, coFloat _learningRate, coFloat _momentum, coFloat _miniBatchFactor)
{
	const coArray<coNeuralTrainingLayer*>& trainingLayers = _trainingNet.GetTrainingLayers();
	for (coNeuralTrainingLayer* trainingLayer : trainingLayers)
	{
		coNeuralLayer* data = trainingLayer->layer;
		const coUint nbInputs = data->GetNbInputs();
		const coUint nbOutputs = data->GetNbOutputs();
		coArray<coFloat>& biasBuffer = data->GetBiasBuffer();
		coArray<coFloat>& weightBuffer = data->GetWeightBuffer();
		coArray<coFloat>& biasDeltas = trainingLayer->biasDeltas;
		coArray<coFloat>& weightDeltas = trainingLayer->weightDeltas;
		const coArray<coFloat>& biasAccs = trainingLayer->biasAccs;
		const coArray<coFloat>& weightAccs = trainingLayer->weightAccs;

		const coFloat decay = 0.0f;//0.000001f;

		coUint weightIndex = 0;
		for (coUint o = 0; o < nbOutputs; ++o)
		{
			// Biases
			{
				const coFloat biasAcc = biasAccs.data[o] * _miniBatchFactor;
				coFloat& bias = biasBuffer.data[o];
				coFloat& biasDelta = biasDeltas.data[o];
				biasDelta = _learningRate * biasAcc + _momentum * biasDelta - decay * _learningRate * bias;
				bias += biasDelta;
				biasAccs.data[o] = 0.0f;
			}

			// Weights
			for (coUint i = 0; i < nbInputs; ++i)
			{
				coFloat& weight = weightBuffer.data[weightIndex];
				coFloat& weightDelta = weightDeltas.data[weightIndex];
				const coFloat weightAcc = weightAccs.data[weightIndex] * _miniBatchFactor;
				weightDelta = _learningRate * weightAcc + _momentum * weightDelta - decay * _learningRate* weight;
				weight += weightDelta;
				weightAccs.data[weightIndex] = 0.0f;
				++weightIndex;
			}
		}
	}
}

coFloat coComputeErrorSum(const coNeuralTrainingNet& _trainingNet, const coArray<coFloat>& _targetOutputs)
{
	coFloat error = 0.0f;
	const coArray<coNeuralTrainingLayer*>& trainingLayers = _trainingNet.GetTrainingLayers();
	coNeuralTrainingLayer* outputTrainingLayer = coBack(trainingLayers);
	const coArray<coFloat>& outputs = outputTrainingLayer->outputs;
	coASSERT(outputs.count == _targetOutputs.count);
	const coUint nbOutputs = outputTrainingLayer->outputs.count;
	for (coUint o = 0; o < nbOutputs; ++o)
	{
		error += coPow2(_targetOutputs[o] - outputs[o]);
	}
	return error;
}

coResult coTrain(coNeuralTrainingNet& _trainingNet, const coNeuralDataSet& _dataSet, const coNeuralTrainingConfig& _config, coUint32& _seed)
{
	// References:
	// - "On Large-Batch Training for Deep Learning: Generalization Gap and Sharp Minima" (arXiv:1609.04836).

	coTRY(_config.Validate(), "Invalid config.");

	const coFloat meanSquareErrorTarget = coPow2(_config.targetError);

	coNeuralNet* net = _trainingNet.GetNet();
	coTRY(net, nullptr);

	const coUint nbNetInputs = net->GetNbInputs();
	const coUint nbNetOutputs = net->GetNbOutputs();

	coDynamicArray<coUint> sampleIndices;
	coResize(sampleIndices, _dataSet.nbSamples);
	for (coUint i = 0; i < sampleIndices.count; ++i)
		sampleIndices[i] = i;

#ifdef coDEBUG
	coDynamicArray<coFloat> errors;
	coReserve(errors, _dataSet.nbSamples);
#endif

	coASSERT(coIsInRange01(_config.sampleRatio));
	const coUint32 nbTrainingSamples = coUint32(_config.sampleRatio * _dataSet.nbSamples);
	coASSERT(nbTrainingSamples < _dataSet.nbSamples);
	const coUint32 nbValidationSamples = _dataSet.nbSamples - nbTrainingSamples;

	const coUint nbMiniBatches = nbTrainingSamples / _config.nbSamplesPerMiniBatch;
	const coFloat miniBatchFactor = 1.0f / _config.nbSamplesPerMiniBatch;

	// For each epoch
	coFloat err = coFloat_inf;
	coUint nbEpochs = 0;
	do
	{
		coClearForEpoch(_trainingNet);

		// Shuffle samples
		coShuffle(sampleIndices, _seed);

		// Train
		{
			coUint tableIndex = 0;
			for (coUint i = 0; i < nbMiniBatches; ++i)
			{
				for (coUint j = 0; j < _config.nbSamplesPerMiniBatch; ++j)
				{
					const coUint sampleIndex = sampleIndices[tableIndex];
					const coArray<coFloat> sampleInputs(_dataSet.inputs.data + sampleIndex * nbNetInputs, nbNetInputs);
					const coArray<coFloat> sampleOutputs(_dataSet.outputs.data + sampleIndex * nbNetOutputs, nbNetOutputs);
					coComputeForwardPass(_trainingNet, sampleInputs);
					coComputeBackwardPass(_trainingNet, sampleInputs, sampleOutputs);
					++tableIndex;
				}
				
				coUpdateWeights(_trainingNet, _config.learningRate, _config.momentum, miniBatchFactor);
			}

			int x = 0;
			++x;
		}

		++nbEpochs;

		// Update validation error
		if (nbEpochs % _config.nbEpochsPerValidation == 0 && nbEpochs < _config.maxNbEpochs)
		{
			err = 0.0f;
			for (coUint i = nbTrainingSamples; i < _dataSet.nbSamples; ++i)
			{
				const coUint sampleIndex = sampleIndices[i];
				const coArray<coFloat> sampleInputs(_dataSet.inputs.data + sampleIndex * nbNetInputs, nbNetInputs);
				const coArray<coFloat> sampleOutputs(_dataSet.outputs.data + sampleIndex * nbNetOutputs, nbNetOutputs);
				coComputeForwardPass(_trainingNet, sampleInputs);
				err += coComputeErrorSum(_trainingNet, sampleOutputs);
			}

			coASSERT(nbValidationSamples);
			err /= coFloat(nbValidationSamples);

#ifdef coDEBUG
			coPushBack(errors, err);
#endif
		}
	} while (err > meanSquareErrorTarget && nbEpochs < _config.maxNbEpochs);

	return true;
}
