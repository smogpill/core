// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "io/stream/coStreamBuffer.h"

class coFileAccess;

class coFileStreamBuffer : public coStreamBuffer
{
	coDECLARE_SUPER(coStreamBuffer);
public:
	enum Mode
	{
		read,
		write
	};

	coFileStreamBuffer();
	~coFileStreamBuffer();

	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coConstString path;
		Mode mode;
	};

	virtual void Reset() override;
	virtual void Flush() override;

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coBool RefillRead();
	coBool RefillWrite();
	void OnImplConstruct();
	void OnImplDestruct();
	coResult OnImplInit(const InitConfig& _config);
	coResult ImplWrite(const coByte* _data, coUint _size8);
	coResult ImplRead(coUint& _readSize8, coByte* _data, coUint _size8);
	coResult ImplFlush();

	Mode mode;
	coDynamicArray<coByte> buffer;
	void* impl;
};
