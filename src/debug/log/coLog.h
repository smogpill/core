// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/string/coDynamicString_f.h"

class coConstString;

enum class coLogType
{
	info,
	warning,
	error
};

void coLog(coLogType _type, const coConstString& _file, coUint _line, const coConstString& _message);
coDynamicString& _coGetTempLogString();

#define coINFO(_message_) coLog(coLogType::info, __FILE__, __LINE__, _coGetTempLogString() << _message_)
#define coWARN(_message_) coLog(coLogType::warning, __FILE__, __LINE__, _coGetTempLogString() << _message_)
#define coERROR(_message_) coLog(coLogType::error, __FILE__, __LINE__, _coGetTempLogString() << _message_)
#define coWARN_NOT_AVAILABLE() coSAFE_SCOPE( coERROR("<not available>"); )
#define coWARN_NOT_TESTED() coWARN("<not tested>")
#define coWARN_NOT_OPTIMIZED() coWARN("<not optimized>")
#define coWARN_NOT_FULLY_FUNCTIONAL coWARN("<not fully functional>")
#define coTODO(_message_) coPRAGMA_MESSAGE("[TODO] " _message_)
#define coHACK(_message_) coPRAGMA_MESSAGE("[HACK] " _message_)
