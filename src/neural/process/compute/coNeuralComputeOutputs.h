// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coArray.h"

class coNeuralLayer;
class coNeuralNet;

void coComputeNeuralOutputs(const coNeuralLayer& _layer, const coArray<coFloat>& _inputs, coArray<coFloat>& _outputs);
void coComputeNeuralOutputs(const coNeuralNet& _net, const coArray<coFloat>& _inputs, coArray<coFloat>& _outputs);