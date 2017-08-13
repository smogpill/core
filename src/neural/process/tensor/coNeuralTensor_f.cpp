// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "neural/pch.h"
#include "neural/process/tensor/coNeuralTensor_f.h"
#include "neural/process/tensor/coNeuralNormalizationConfig.h"
#include "neural/network/coNeuralTensor.h"
#include "debug/log/coAssert.h"
#include "math/scalar/coFloat_f.h"

void coNormalizeSet(coArray<coFloat>& _values, coUint _nbElements, const coNeuralNormalizationConfig& _config)
{
	const coArray<coFloat>& means = _config.means;
	const coArray<coFloat>& deviations = _config.deviations;
	const coUint nbX = means.count;
	coASSERT(means.count == deviations.count);
	coASSERT(_values.count == nbX * _nbElements);
	_nbElements;

	const coFloat targetMean = 0.0f;
	const coFloat targetDeviation = 1.0f;
	
	for (coUint i = 0; i < nbX; ++i)
	{
		const coFloat mean = means[i];
		const coFloat deviation = deviations[i];
		coASSERT(deviation >= 0.0f);
		const coFloat a = targetDeviation / deviation;
		for (coUint index = i; index < _values.count; index += nbX)
		{
			_values[index] = (_values[index] - mean) * a + targetMean; // [-1, 1]
		}
	}
}

void coNormalize(coArray<coFloat>& _values, const coNeuralNormalizationConfig& _config)
{
	coNormalizeSet(_values, 1, _config);
}

void coDenormalizeSet(coArray<coFloat>& _values, coUint _nbElements, const coNeuralNormalizationConfig& _config)
{
	const coArray<coFloat>& means = _config.means;
	const coArray<coFloat>& deviations = _config.deviations;
	const coUint nbX = means.count;
	coASSERT(means.count == deviations.count);
	coASSERT(_values.count == nbX * _nbElements);
	_nbElements;

	const coFloat targetMean = 0.0f;
	const coFloat targetDeviation = 1.0f;
	for (coUint i = 0; i < nbX; ++i)
	{
		const coFloat mean = means[i];
		const coFloat deviation = deviations[i];
		coASSERT(deviation > mean);
		const coFloat a = deviation / targetDeviation;
		for (coUint index = i; index < _values.count; index += nbX)
		{
			_values[index] = (_values[index] - targetMean) * a + mean;
		}
	}
}

void coDenormalize(coArray<coFloat>& _values, const coNeuralNormalizationConfig& _config)
{
	coDenormalizeSet(_values, 1, _config);
}

coFloat coComputeMeanSquaredError(const coArray<coFloat>& _values, const coArray<coFloat>& _targets)
{
	coASSERT(_values.count == _targets.count);
	coFloat error = 0.0f;
	const coUint nb = _values.count;
	if (nb)
	{
		for (coUint i = 0; i < nb; ++i)
		{
			error += coPow2(_targets[i] - _values[i]);
		}
		error /= nb;
	}
	return error;
}
