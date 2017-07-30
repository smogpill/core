// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "neural/pch.h"
#include "neural/network/coNeuralNet.h"
#include "neural/network/coNeuralLayer.h"
#include "neural/process/training/coNeuralTrainingLayer.h"
#include "neural/process/training/coNeuralDataSet.h"
#include "neural/process/compute/coNeuralComputeOutputs.h"
#include "neural/process/coNeuralSigmoid_f.h"
#include "lang/result/coResult_f.h"
#include "math/scalar/coFloat_f.h"
#include "math/scalar/coUint32_f.h"

coNeuralNet::coNeuralNet(const coArray<coNeuralLayer*> _datas)
{
	coPushBackArray(layers, _datas);
}

coResult coNeuralNet::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	coTRY(layers.count, nullptr);
	return true;
}

coResult coNeuralNet::Compute(const coArray<coFloat>& _inputs, coArray<coFloat>& _outputs)
{
	coTRY(layers.count > 0, nullptr);
	coTRY(layers[0]->GetNbInputs() == _inputs.count, nullptr);
	coTRY(coBack(layers)->GetNbOutputs() == _outputs.count, nullptr);

	// Compute max nb outputs
	coUint nbMaxOutputs = 0;
	for (const coNeuralLayer* layer : layers)
	{
		nbMaxOutputs = coMax(nbMaxOutputs, layer->GetNbOutputs());
	}

	coDynamicArray<coFloat> tempInputs;
	coDynamicArray<coFloat> tempOutputs;
	coResize(tempInputs, nbMaxOutputs);
	coResize(tempOutputs, nbMaxOutputs);

	const coArray<coFloat>* in = &_inputs;
	coArray<coFloat>* out = &tempOutputs;

	for (const coNeuralLayer* layer : layers)
	{
		coArray<coFloat> inputs = coArray<coFloat>(in->data, layer->GetNbInputs());
		coArray<coFloat> outputs = coArray<coFloat>(out->data, layer->GetNbOutputs());
		coComputeNeuralLayerOutputs(*layer, inputs, outputs);
		coSwap(tempInputs, tempOutputs);
		in = &tempInputs;
		out = &tempOutputs;
	}

	for (coUint i = 0; i < _outputs.count; ++i)
	{
		_outputs[i] = (*in)[i];
	}

	return true;
}

coResult coNeuralNet::Train(const coNeuralDataSet& _dataSet, coFloat _targetError, coUint _nbMaxEpochs)
{
	// References:
	// - "On Large-Batch Training for Deep Learning: Generalization Gap and Sharp Minima" (arXiv:1609.04836).

	coTRY(_targetError >= 0.0f, nullptr);

	const coFloat learnRate = 0.4f;
	const coFloat momentum = 0.9f;

	//coTRY(learnRate < 1.0f, nullptr);

	const coUint nbNetInputs = GetNbInputs();
	const coUint nbNetOutputs = GetNbOutputs();

	// Init weights
	{
		coUint32 seed = 1;
		for (coNeuralLayer* layer : layers)
		{
			for (coFloat& weight : layer->GetWeightBuffer())
			{
				weight = coRand11(seed);
			}
			for (coFloat& bias : layer->GetBiasBuffer())
			{
				bias = coRand11(seed);
			}
		}
	}

	// Training layers
	coDynamicArray<coNeuralTrainingLayer*> trainingLayers;
	{
		coReserve(trainingLayers, layers.count);
		coArray<coFloat> inputs;
		for (coNeuralLayer* layer : layers)
		{
			coNeuralTrainingLayer* trainingLayer = new coNeuralTrainingLayer();
			trainingLayer->layer = layer;
			const coUint nbInputs = layer->GetNbInputs();
			const coUint nbOutputs = layer->GetNbOutputs();
			coResize(trainingLayer->weightDeltas, nbInputs * nbOutputs);
			coFill(trainingLayer->weightDeltas, 0.0f);
			coResize(trainingLayer->biasDeltas, nbOutputs);
			coFill(trainingLayer->biasDeltas, 0.0f);
			coResize(trainingLayer->outputs, nbOutputs);
			coResize(trainingLayer->gradients, nbOutputs);
			coPushBack(trainingLayers, trainingLayer);
			trainingLayer->inputs.data = inputs.data;
			trainingLayer->inputs.count = inputs.count;
			inputs.data = trainingLayer->outputs.data;
			inputs.count = trainingLayer->outputs.count;
		}
	}

	coFloat err;
	coUint nbEpochs = 0;
	do
	{
		err = 0.0f;
		for (coUint s = 0; s < _dataSet.nbSamples; ++s)
		{
			coArray<coFloat> sampleInputs(_dataSet.inputs.data + s * nbNetInputs, nbNetInputs);
			coArray<coFloat> sampleOutputs(_dataSet.outputs.data + s * nbNetOutputs, nbNetOutputs);

			// Forward pass
			{
				coArray<coFloat> inputs = sampleInputs;
				for (coNeuralTrainingLayer* trainingLayer : trainingLayers)
				{
					coNeuralLayer* layer = trainingLayer->layer;
					coComputeNeuralLayerOutputs(*layer, inputs, trainingLayer->outputs);
					inputs = trainingLayer->outputs;
				}
			}

			// Backward pass
			{
				// Compute gradients
				{
					// Last layer
					{
						coNeuralTrainingLayer* trainingLayer = coBack(trainingLayers);
						const coUint nbOutputs = trainingLayer->outputs.count;
						coArray<coFloat>& gradients = trainingLayer->gradients;
						const coArray<coFloat>& values = trainingLayer->outputs;
						for (coUint i = 0; i < nbOutputs; ++i)
						{
							const coFloat value = values[i];
							const coFloat targetValue = sampleOutputs[i];
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

				// Update weights
				coArray<coFloat> layerInputs(sampleInputs);
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
							curDelta = learnRate * gradient;
							biasBuffer[k] += curDelta + momentum * prevDelta;
						}

						// Weights
						for (coUint l = 0; l < nbInputs; ++l)
						{
							coFloat& curDelta = weightDeltas[weightIndex];
							const coFloat prevDelta = curDelta;
							curDelta = learnRate * gradient * layerInputs[l];
							weightBuffer[weightIndex] += curDelta + momentum * prevDelta;
							++weightIndex;
						}
					}

					layerInputs = trainingLayer->outputs;
				}
			}

			// Compute error
			{
				coNeuralTrainingLayer* lastTrainingLayer = coBack(trainingLayers);
				const coArray<coFloat>& outputs = lastTrainingLayer->outputs;
				for (coUint i = 0; i < nbNetOutputs; ++i)
				{
					err += coPow2(sampleOutputs[i] - outputs[i]);
				}
			}
		}

		coASSERT(_dataSet.nbSamples);
		err /= coFloat(_dataSet.nbSamples);
		err *= 0.5f;

		++nbEpochs;
	}
	while (err > _targetError && nbEpochs < _nbMaxEpochs);

	for (coNeuralTrainingLayer* trainingLayer : trainingLayers)
		delete trainingLayer;

	return true;
}

coUint coNeuralNet::GetNbInputs() const
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

coUint coNeuralNet::GetNbOutputs() const
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
