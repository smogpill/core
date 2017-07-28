// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coNeuralLayer;

class coNeuralNet : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	class DataSet
	{
	public:
		coUint nbSamples;
		coArray<coFloat> inputs;
		coArray<coFloat> outputs;
	};
	coResult Compute(const coArray<coFloat>& _inputs, coArray<coFloat>& _outputs);
	coResult Train(const DataSet& _dataSet, coFloat _targetError);
	coUint GetNbInputs() const;
	coUint GetNbOutputs() const;

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:

	coDynamicArray<coNeuralLayer*> layers;
};
