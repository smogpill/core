// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

template<class T> class coArray;
class coNeuralLayer;
class coNeuralNet;

void coComputeOutputs(const coNeuralNet& _net, const coArray<coFloat>& _inputs, coArray<coFloat>& _outputs);
