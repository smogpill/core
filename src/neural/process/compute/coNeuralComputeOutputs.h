// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coArray.h"

class coNeuralLayer;

//coResult coCompute(const coArray<coFloat>& _inputs, coArray<coFloat>& _outputs);
void coComputeNeuralLayerOutputs(const coNeuralLayer& _layerData, const coArray<coFloat>& _inputs, coArray<coFloat>& _outputs);
