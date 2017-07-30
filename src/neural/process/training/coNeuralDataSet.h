// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coArray.h"

class coNeuralDataSet
{
public:
	coUint nbSamples;
	coArray<coFloat> inputs;
	coArray<coFloat> outputs;
};
