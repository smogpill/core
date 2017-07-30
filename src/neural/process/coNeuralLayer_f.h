// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

template<class T> class coArray;
class coNeuralLayer;

void coRandomizeWeightsAndBiases(coNeuralLayer& _this, coUint32& _seed);
void coComputeNeuralOutputs(const coNeuralLayer& _this, const coArray<coFloat>& _inputs, coArray<coFloat>& _outputs);
