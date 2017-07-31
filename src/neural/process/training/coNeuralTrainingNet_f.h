// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/result/coResult.h"

template<class T> class coArray;
class coNeuralTrainingNet;
class coNeuralDataSet;

coResult coTrain(coNeuralTrainingNet& _net, const coNeuralDataSet& _dataSet, coFloat _targetError, coUint _nbMaxEpochs);
