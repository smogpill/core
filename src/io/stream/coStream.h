// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coStreamBuffer;

class coStream : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		InitConfig();
		coStreamBuffer* buffer;
	};
	coStream();

	virtual void Clear();
	coResult GetResult() const;
	coStreamBuffer* GetBuffer() const { return buffer; }
	void Flush();

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

	coStreamBuffer* buffer;
};
