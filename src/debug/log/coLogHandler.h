// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "pattern/singleton/coSingleton.h"
#include "debug/log/coLog.h"

class coConstString;

class coLogHandler
{
	coDECLARE_SINGLETON(coLogHandler);
public:
	virtual ~coLogHandler() {}

	virtual void Log(coLogType _type, const coConstString& _file, coUint _line, const coConstString& _message) = 0;
};
