// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coDynamicArray.h"
#include "pattern/object/coObject.h"

class coNeuralTrainingLayer;
class coNeuralNet;

class coNeuralTrainingNet : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	coNeuralTrainingNet(coNeuralNet& _net);
	virtual ~coNeuralTrainingNet();

	coNeuralNet* GetNet() const { return net; }
	const coArray<coNeuralTrainingLayer*> GetTrainingLayers() const { return trainingLayers; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coNeuralNet* net;
	coDynamicArray<coNeuralTrainingLayer*> trainingLayers;
};