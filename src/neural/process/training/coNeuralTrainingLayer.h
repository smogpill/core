// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coDynamicArray.h"

class coNeuralLayer;

class coNeuralTrainingLayer
{
public:
	coNeuralTrainingLayer();
	coNeuralLayer* layer;
	coDynamicArray<coFloat> weightDeltas;
	coDynamicArray<coFloat> biasDeltas;
	coDynamicArray<coFloat> biasAccs;
	coDynamicArray<coFloat> weightAccs;
	coDynamicArray<coFloat> deltas;
	coDynamicArray<coFloat> biasRunningAverages; // used by RMSProp
	coDynamicArray<coFloat> weightRunningAverages; // used by RMSProp
	coDynamicArray<coFloat> outputs;
};
