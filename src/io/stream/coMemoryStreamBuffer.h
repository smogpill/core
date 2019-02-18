// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "io/stream/coStreamBuffer.h"

class coMemoryStreamBuffer : public coStreamBuffer
{
	coDECLARE_SUPER(coStreamBuffer);
public:

	virtual void Reset() override;

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coBool Refill();

	coDynamicArray<coByte> buffer;
};
