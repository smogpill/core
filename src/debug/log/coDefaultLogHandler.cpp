// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "debug/pch.h"
#include "debug/log/coDefaultLogHandler.h"
#include "container/string/coConstString_f.h"
#include "container/string/coDynamicString_f.h"

void coDefaultLogHandler::Log(_coLogType _type, const coConstString& _file, coUint _line, const coConstString& _message)
{
	coConstString tag;
	switch (_type)
	{
	case _coLogType::INFO:
	{
		tag = "<Info";
		break;
	}
	case _coLogType::WARNING:
	{
		tag = "Warning";
		break;
	}
	case _coLogType::ERROR:
	{
		tag = "Error";
		break;
	}
	default:
	{
		break;
	}
	}

	coDynamicString s;
	s << _file << "(" << _line << "): <" << tag << "> " << _message;
	//printf(s);
}
