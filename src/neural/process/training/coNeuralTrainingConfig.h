// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/result/coResult.h"

class coNeuralTrainingConfig
{
public:
	coNeuralTrainingConfig();
	coResult Validate() const;

	coFloat targetError;
	coUint maxNbEpochs;
	coFloat sampleRatio;
	coFloat learningRate;
	coFloat decay;
	coFloat adamBeta1;
	coFloat adamBeta2;
	coUint nbEpochsPerValidation;
	coUint nbSamplesPerMiniBatch;
};
