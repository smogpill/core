// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coDefaultLogHandler;

class coApp : public coObject
{
	typedef coObject Super;
public:
	coApp();
	virtual ~coApp();

	class InitConfig : public Super::InitConfig
	{
	};
protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coDefaultLogHandler* defaultLogHandler;
};
