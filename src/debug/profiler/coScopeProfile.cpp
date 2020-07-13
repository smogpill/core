// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "debug/pch.h"
#include "coScopeProfile.h"
#include <platform/coOs.h>
#include <debug/log/coLog.h>

_coScopeProfile::_coScopeProfile(const coConstString& name)
	: startTime(coGetTimeStamp())
	, name(name)
{
}

_coScopeProfile::~_coScopeProfile()
{
	const coUint64 endTime = coGetTimeStamp();
	const coFloat seconds = coFloat(coGetSeconds(endTime - startTime));
	coINFO("> " << name << ": " << seconds << " s");
}
