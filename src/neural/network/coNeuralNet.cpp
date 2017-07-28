// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "neural/pch.h"
#include "neural/network/coNeuralNet.h"
#include "neural/network/coNeuralLayer.h"
#include "neural/network/coNeuralLayerData.h"
#include "lang/result/coResult_f.h"
#include "math/scalar/coFloat_f.h"
#include "math/scalar/coUint32_f.h"

coResult coNeuralNet::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	return true;
}

coResult coNeuralNet::Compute(const coArray<coFloat>& _inputs, coArray<coFloat>& _outputs)
{
	coTRY(layers.count > 0, nullptr);
	coTRY(layers[0]->GetData()->GetNbInputs() == _inputs.count, nullptr);
	coTRY(coBack(layers)->GetData()->GetNbOutputs() == _outputs.count, nullptr);

	// Compute max nb outputs
	coUint nbMaxOutputs = 0;
	for (const coNeuralLayer* layer : layers)
	{
		const coNeuralLayerData* data = layer->GetData();
		nbMaxOutputs = coMax(nbMaxOutputs, data->GetNbOutputs());
	}

	coDynamicArray<coFloat> tempInputs;
	coDynamicArray<coFloat> tempOutputs;
	coResize(tempInputs, nbMaxOutputs);
	coResize(tempOutputs, nbMaxOutputs);

	const coArray<coFloat>* in = &_inputs;
	coArray<coFloat>* out = &tempOutputs;

	for (const coNeuralLayer* layer : layers)
	{
		layer->Compute(*in, *out);
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

coResult coNeuralNet::Train(const DataSet& _dataSet, coFloat _targetError)
{
	coTRY(_targetError >= 0.0f, nullptr);

	const coFloat learnRate = 0.4f;
	const coFloat momentum = 0.9f;

	// References:
	// - "On Large-Batch Training for Deep Learning: Generalization Gap and Sharp Minima" (arXiv:1609.04836).

	coUint nbEpochs = 0;

	const coUint nbNetInputs = GetNbInputs();
	const coUint nbNetOutputs = GetNbOutputs();

	// Training layers
	class TrainingLayer
	{
	public:
		coNeuralLayer* layer;
		coDynamicArray<coFloat> weightDeltas;
		coDynamicArray<coFloat> biasDeltas;
		coDynamicArray<coFloat> gradients;
		coDynamicArray<coFloat> outputs;
		coArray<coFloat> inputs;
	};
	coDynamicArray<TrainingLayer*> trainingLayers;
	{
		coReserve(trainingLayers, layers.count);
		coArray<coFloat> inputs;
		for (coNeuralLayer* layer : layers)
		{
			const coNeuralLayerData* data = layer->GetData();
			TrainingLayer* trainingLayer = new TrainingLayer();
			trainingLayer->layer = layer;
			const coUint nbInputs = data->GetNbInputs();
			const coUint nbOutputs = data->GetNbOutputs();
			coResize(trainingLayer->weightDeltas, nbInputs * nbOutputs);
			coResize(trainingLayer->biasDeltas, nbOutputs);
			coResize(trainingLayer->outputs, nbOutputs);
			coResize(trainingLayer->gradients, nbOutputs);
			coPushBack(trainingLayers, trainingLayer);
			trainingLayer->inputs.data = inputs.data;
			trainingLayer->inputs.count = inputs.count;
			inputs.data = trainingLayer->outputs.data;
			inputs.count = trainingLayer->outputs.count;
		}
	}

	coFloat err = 1.0f;
	do
	{
		for (coUint i = 0; i < _dataSet.nbSamples; ++i)
		{
			coArray<coFloat> sampleInputs(_dataSet.inputs.data + i * nbNetInputs, nbNetInputs);
			coArray<coFloat> sampleOutputs(_dataSet.outputs.data + i * nbNetOutputs, nbNetOutputs);

			// Forward pass
			for (coNeuralLayer* layer : layers)
			{
				coArray<coFloat> inputs;
				coArray<coFloat> outputs;
				layer->Compute(inputs, outputs);
			}

			// Backward pass
			{
				// Compute gradients
				{
					// Last layer
					{
						TrainingLayer* trainingLayer = coBack(trainingLayers);
						const coUint nbOutputs = trainingLayer->outputs.count;
						coArray<coFloat>& gradients = trainingLayer->gradients;
						const coArray<coFloat>& values = trainingLayer->outputs;
						for (coUint k = 0; k < nbOutputs; ++k)
						{
							const coFloat value = values[k];
							const coFloat targetValue = sampleOutputs[k];
							const coFloat error = targetValue - value;
							gradients[k] = error * coNeuralLayer::ComputeSigmoidDerivative(value);
						}
					}

					// Rest of the layers
					for (coUint j = layers.count - 2; j >= 0; --j)
					{
						TrainingLayer* trainingLayer = trainingLayers[j];
						TrainingLayer* nextTrainingLayer = trainingLayers[j + 1];
						coArray<coFloat>& gradients = trainingLayer->gradients;
						const coArray<coFloat>& nextGradients = nextTrainingLayer->gradients;
						const coArray<coFloat>& values = trainingLayer->outputs;

						const coNeuralLayerData* data = trainingLayer->layer->GetData();
						const coNeuralLayerData* nextData = nextTrainingLayer->layer->GetData();
						const coArray<coFloat>& nextWeights = nextData->GetWeightBuffer();
						const coUint nbOutputs = data->GetNbOutputs();
						const coUint nbNextOutputs = nextData->GetNbOutputs();
						for (coUint k = 0; k < nbOutputs; ++k)
						{
							coFloat sum = 0.0f;
							for (coUint l = 0; l < nbNextOutputs; ++l)
							{
								const coUint weightIndex = l * nbOutputs + k;
								sum += nextGradients[l] * nextWeights[weightIndex];
							}
							const coFloat value = values[k];
							gradients[k] = sum * coNeuralLayer::ComputeSigmoidDerivative(value);
						}
					}
				}

				// Update weights
				coArray<coFloat> layerInputs(sampleInputs);
				for (TrainingLayer* trainingLayer : trainingLayers)
				{
					coNeuralLayerData* data = trainingLayer->layer->GetData();
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
							coFloat& delta = weightDeltas[weightIndex];
							const coFloat prevDelta = delta;
							delta = learnRate * gradient * layerInputs[l];
							weightBuffer[weightIndex] += delta + momentum * prevDelta;
							++weightIndex;
						}
					}

					layerInputs.data = trainingLayer->outputs.data;
					layerInputs.count = trainingLayer->outputs.count;
				}
			}
		}

		++nbEpochs;
	}
	while (err > _targetError);

	for (TrainingLayer* trainingLayer : trainingLayers)
		delete trainingLayer;

	return true;
}

coUint coNeuralNet::GetNbInputs() const
{
	if (layers.count)
	{
		const coNeuralLayer* firstLayer = layers[0];
		const coNeuralLayerData* data = firstLayer->GetData();
		coASSERT(data);
		return data->GetNbInputs();
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
		const coNeuralLayerData* data = firstLayer->GetData();
		coASSERT(data);
		return data->GetNbOutputs();
	}
	else
	{
		return 0;
	}
}
