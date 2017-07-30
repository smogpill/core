// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "neural/pch.h"
#include "neural/network/coNeuralLayer.h"
#include "neural/network/coNeuralLayerData.h"
#include "lang/result/coResult_f.h"
#include "math/scalar/coFloat_f.h"
#include "math/scalar/coUint32_f.h"

coNeuralLayer::coNeuralLayer(coNeuralLayerData& _data)
	: data(&_data)
{

}

coNeuralLayer::~coNeuralLayer()
{
}

coResult coNeuralLayer::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	coTRY(data, nullptr);

	return true;
}

void coNeuralLayer::Compute(const coArray<coFloat>& _inputs, coArray<coFloat>& _outputs) const
{
	coASSERT(data);
	const coUint nbInputs = data->GetNbInputs();
	const coUint nbOutputs = data->GetNbOutputs();
	coASSERT(_inputs.count == nbInputs);
	coASSERT(_outputs.count == nbOutputs);

	const coArray<coFloat>& weightBuffer = data->GetWeightBuffer();
	const coArray<coFloat>& biasBuffer = data->GetBiasBuffer();

	coUint weightIndex = 0;
	for (coUint j = 0; j < nbOutputs; ++j)
	{
		coFloat o = biasBuffer[j];
		for (coUint i = 0; i < nbInputs; ++i)
		{
			o += _inputs[i] * weightBuffer[weightIndex];
			++weightIndex;
		}
		_outputs[j] = ComputeSigmoid(o);
	}
}

coFloat coNeuralLayer::ComputeSigmoid(coFloat _x)
{
	return _x < -45.0f ? 0.0f : _x > 45.0f ? 1.0f : 1.0f / (1.0f + coExp(-_x));

	// Not used for now, because:
	// - Have to find a good derivative form
	// - Have to understand how the range change from [0, 1] to [-1, 1] affects the inputs and outputs.
	// tanh = (exp(2x) - 1) / (exp(2x) + 1)
	// tanh is better than the usual logistic function because it avoids saturation at near 0. 
// 	const coFloat a = 2.0f * _x;
// 	const coFloat b = coExp(a);
// 	return (b - 1.0f) / (b + 1.0f);
}

coFloat coNeuralLayer::ComputeSigmoidDerivative(coFloat _x)
{
	return _x * (1 - _x);
}
