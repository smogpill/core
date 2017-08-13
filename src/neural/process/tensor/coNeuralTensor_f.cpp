// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "neural/pch.h"
#include "neural/process/tensor/coNeuralTensor_f.h"
#include "neural/process/tensor/coNeuralNormalizationConfig.h"
#include "neural/network/coNeuralTensor.h"
#include "debug/log/coAssert.h"
#include "math/scalar/coFloat_f.h"

void coNormalize(coArray<coFloat>& _values, const coNeuralNormalizationConfig& _config)
{
	coASSERT(_values.count == _config.mins.count);
	coASSERT(_values.count == _config.maxs.count);
	const coArray<coFloat>& mins = _config.mins;
	const coArray<coFloat>& maxs = _config.maxs;
	const coUint nb = _values.count;
	for (coUint i = 0; i < nb; ++i)
	{
		const coFloat min = mins[i];
		const coFloat max = maxs[i];
		coASSERT(max > min);
		const coFloat range = max - min;
		_values[i] = (_values[i] - min) / range;
	}
}

void coDenormalize(coArray<coFloat>& _values, const coNeuralNormalizationConfig& _config)
{
	coASSERT(_values.count == _config.mins.count);
	coASSERT(_values.count == _config.maxs.count);
	const coArray<coFloat>& mins = _config.mins;
	const coArray<coFloat>& maxs = _config.maxs;
	const coUint nb = mins.count;
	for (coUint i = 0; i < nb; ++i)
	{
		const coFloat min = mins[i];
		const coFloat max = maxs[i];
		coASSERT(max > min);
		const coFloat range = max - min;
		_values[i] = _values[i] * range + min;
	}
}

void coNormalizeSet(coArray<coFloat>& _values, coUint _nbElements, const coNeuralNormalizationConfig& _config)
{
	const coArray<coFloat>& mins = _config.mins;
	const coArray<coFloat>& maxs = _config.maxs;
	const coUint nbX = mins.count;
	coASSERT(mins.count == maxs.count);
	coASSERT(_values.count == nbX * _nbElements);
	_nbElements;
	
	for (coUint i = 0; i < nbX; ++i)
	{
		const coFloat min = mins[i];
		const coFloat max = maxs[i];
		coASSERT(max > min);
		const coFloat invRange = 1.0f / (max - min);
		for (coUint index = i; index < _values.count; index += nbX)
		{
			_values[index] = (_values[index] - min) * invRange;
		}
	}
}

void coDenormalizeSet(coArray<coFloat>& _values, coUint _nbElements, const coNeuralNormalizationConfig& _config)
{
	const coArray<coFloat>& mins = _config.mins;
	const coArray<coFloat>& maxs = _config.maxs;
	const coUint nbX = mins.count;
	coASSERT(mins.count == maxs.count);
	coASSERT(_values.count == nbX * _nbElements);
	_nbElements;
	for (coUint i = 0; i < nbX; ++i)
	{
		const coFloat min = mins[i];
		const coFloat max = maxs[i];
		coASSERT(max > min);
		const coFloat range = max - min;
		for (coUint index = i; index < _values.count; index += nbX)
		{
			_values[index] = _values[index] * range + min;
		}
	}
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
