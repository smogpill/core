// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coArray.h"
#include "neural/network/coNeuralTensor.h"

class coNeuralDataSet
{
public:
	coNeuralTensor2 inputs;
	coNeuralTensor2 outputs;
};
