// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "debug/pch.h"
#include "coScopeTime.h"
#include <platform/coOs.h>
#include <debug/log/coLog.h>

_coScopeTime::_coScopeTime(const coConstString& name)
	: startTime(coGetTimeStamp())
	, name(name)
{
}

_coScopeTime::~_coScopeTime()
{
	const coUint64 endTime = coGetTimeStamp();
	const coFloat seconds = coFloat(coGetSeconds(endTime - startTime));
	coINFO("> " << name << ": " << seconds << " s");
}
