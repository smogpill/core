// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

template<class T> class coArray;
class coNeuralNet;

void coResetWeightsAndBiases(coNeuralNet& _this, coUint32& _seed);
void coComputeNeuralOutputs(const coNeuralNet& _this, const coArray<coFloat>& _inputs, coArray<coFloat>& _outputs);
