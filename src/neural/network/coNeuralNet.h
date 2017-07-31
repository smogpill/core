// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coNeuralLayerOld;
class coNeuralLayer;
class coNeuralDataSet;

class coNeuralNet : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	coNeuralNet(const coArray<coNeuralLayer*> _layers);
	virtual ~coNeuralNet() {}
	const coArray<coNeuralLayer*> GetLayers() const { return layers; }
	coUint GetNbInputs() const;
	coUint GetNbOutputs() const;

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coArray<coNeuralLayer*> layers;
};
