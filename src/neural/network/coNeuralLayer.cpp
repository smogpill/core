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
