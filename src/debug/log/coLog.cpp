// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "debug/pch.h"
#include "debug/log/coLog.h"
#include "debug/log/coLogHandler.h"
#include "container/string/coDynamicString_f.h"

void coLog(coLogType _type, const coConstString& _file, coUint _line, const coConstString& _message)
{
	coLogHandler* handler = coLogHandler::instance;
	if (handler)
	{
		handler->Log(_type, _file, _line, _message);
	}
}

coDynamicString& _coGetTempLogString()
{
	static coDynamicString str;
	coClear(str);
	return str;
}
