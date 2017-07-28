// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coNeuralLayerData : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	coNeuralLayerData();
	virtual ~coNeuralLayerData();

	coUint GetNbInputs() const { return nbInputs; }
	coUint GetNbOutputs() const { return nbOutputs; }
	const coArray<coFloat>& GetWeightBuffer() const { return weightBuffer; }
	coArray<coFloat>& GetWeightBuffer() { return weightBuffer; }
	const coArray<coFloat>& GetBiasBuffer() const { return biasBuffer; }
	coArray<coFloat>& GetBiasBuffer() { return biasBuffer; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coUint nbInputs;
	coUint nbOutputs;
	coDynamicArray<coFloat> weightBuffer;
	coDynamicArray<coFloat> biasBuffer;
};
