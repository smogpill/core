// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "debug/log/coLogHandler.h"

class coDefaultLogHandler : public coLogHandler
{
public:
	virtual void Log(_coLogType _type, const coConstString& _file, coUint _line, const coConstString& _message) override;

private:
};
