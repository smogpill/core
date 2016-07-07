// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coConstString;
class coDynamicString;

enum class _coLogType
{
	INFO,
	WARNING,
	ERROR
};

void _coLog(_coLogType _type, const coConstString& _file, coUint _line, const coConstString& _message);
coDynamicString& _coGetLogString();

#define coINFO(_message_) _coLog(_coLogType::INFO, __FILE__, __LINE__, _coGetLogString() << _message_)
#define coWARN(_message_) _coLog(_coLogType::WARNING, __FILE__, __LINE__, _coGetLogString() << _message_)
#define coERROR(_message_) _coLog(_coLogType::ERROR, __FILE__, __LINE__, _coGetLogString() << _message_)
#define coWARN_NOT_AVAILABLE() coSAFE_SCOPE( coERROR("<not available>"); )
#define coWARN_NOT_TESTED() coWARN("<not tested>")
#define coWARN_NOT_OPTIMIZED() coWARN("<not optimized>")
#define coWARN_NOT_FULLY_FUNCTIONAL coWARN("<not fully functional>")
#define coTODO(_message_) coPRAGMA_MESSAGE("[TODO] " _message_)
#define coHACK(_message_) coPRAGMA_MESSAGE("[HACK] " _message_)
