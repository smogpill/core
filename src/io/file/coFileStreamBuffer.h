// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "io/stream/coStreamBuffer.h"

class coFileAccess;

class coFileStreamBuffer : public coStreamBuffer
{
	coDECLARE_SUPER(coStreamBuffer);
public:
	coFileStreamBuffer();
	~coFileStreamBuffer();

	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coFileAccess* fileAccess;
	};

	virtual void Reset() override;
	virtual void Flush() override;

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coBool RefillRead();
	coBool RefillWrite();

	coFileAccess* fileAccess;
	coDynamicArray<coByte> buffer;
};
