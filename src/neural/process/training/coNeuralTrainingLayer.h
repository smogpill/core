// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coDynamicArray.h"

class coNeuralLayer;

class coNeuralTrainingLayer
{
public:
	coNeuralLayer* layer;
	coDynamicArray<coFloat> weightDeltas;
	coDynamicArray<coFloat> biasDeltas;
	coDynamicArray<coFloat> gradients;
	coDynamicArray<coFloat> outputs;
	coArray<coFloat> inputs;
};
