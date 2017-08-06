// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "neural/pch.h"
#include "neural/process/training/coNeuralTrainingConfig.h"
#include "lang/result/coResult_f.h"
#include "math/scalar/coFloat_f.h"

coNeuralTrainingConfig::coNeuralTrainingConfig()
	: targetError(0.01f)
	, maxNbEpochs(1000)
	, sampleRatio(0.6f)
	, learningRate(0.03f)
	, momentum(0.1f)
	, decay(0.0f)
	, nbEpochsBeforeValidation(16)
	, miniBatchSize(16)
{
}

coResult coNeuralTrainingConfig::Validate() const
{
	coTRY(targetError >= 0.0f, nullptr);
	coTRY(coIsInRange01(sampleRatio), nullptr);
	coTRY(learningRate >= 0.0f, nullptr);
	coTRY(coIsInRange01(momentum), nullptr);
	coTRY(coIsInRange01(decay), nullptr);
	coTRY(miniBatchSize > 0, nullptr);
	return true;
}
