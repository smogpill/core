// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coArray.h"

class coNeuralNormalizationConfig
{
public:
	coArray<coFloat> inputMins;
	coArray<coFloat> inputMaxs;
	coArray<coFloat> outputMins;
	coArray<coFloat> outputMaxs;
};
