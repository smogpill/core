// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coNeuralLayerData;

class coNeuralLayer : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	coNeuralLayer(coNeuralLayerData& _data);
	virtual ~coNeuralLayer();

	const coNeuralLayerData* GetData() const { return data; }
	coNeuralLayerData* GetData() { return data; }

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coNeuralLayerData* data;
};
