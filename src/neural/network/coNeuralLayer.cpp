// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "neural/pch.h"
#include "neural/network/coNeuralLayer.h"
#include "lang/result/coResult_f.h"

coNeuralLayer::coNeuralLayer(coUint _nbInputs, coUint _nbOutputs)
	: nbInputs(_nbInputs)
	, nbOutputs(_nbOutputs)
{

}

coResult coNeuralLayer::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	coResize(weightBuffer, nbOutputs * nbInputs);
	coResize(biasBuffer, nbOutputs);
	coFill(weightBuffer, 0.0f);
	coFill(biasBuffer, 0.0f);
	return true;
}
