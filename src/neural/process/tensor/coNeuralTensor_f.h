// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

template<class T> class coArray;

class coNeuralNormalizationConfig;

void coNormalize(coArray<coFloat>& _values, const coNeuralNormalizationConfig& _config);
void coNormalizeSet(coArray<coFloat>& _values, coUint _nbElements, const coNeuralNormalizationConfig& _config);
void coDenormalize(coArray<coFloat>& _values, const coNeuralNormalizationConfig& _config);
void coDenormalizeSet(coArray<coFloat>& _values, coUint _nbElements, const coNeuralNormalizationConfig& _config);
coFloat coComputeMeanSquaredError(const coArray<coFloat>& _values, const coArray<coFloat>& _targets);

