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
	case _coLogType::info:
	{
		tag = "Info";
		break;
	}
	case _coLogType::warning:
	{
		tag = "Warning";
		break;
	}
	case _coLogType::error:
	{
		tag = "Error";
		break;
	}
	default:
	{
		break;
	}
	}

	const coBool error = _type >= _coLogType::warning;

	const coConstString& msg = _message.count ? _message : "<no message>";
	coDynamicString outputMsg;
	if (error)
	{
		outputMsg << _file << "(" << _line << "): [" << tag << "] " << msg << "\n";
		fwrite(outputMsg.data, sizeof(*outputMsg.data), outputMsg.count, stderr);
	}
	else
	{
		outputMsg << msg << "\n";
		fwrite(outputMsg.data, sizeof(*outputMsg.data), outputMsg.count, stdout);
	}

#ifdef coMSWINDOWS
	coNullTerminate(outputMsg);
	::OutputDebugStringA(outputMsg.data);
#endif
}
