// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/string/coConstString.h>
#include "coProfile.h"

class _coScopeTime
{
public:
	_coScopeTime(const coConstString& name);
	~_coScopeTime();

private:
	coUint64 startTime;
	const coConstString name;
};

#define coLOG_SCOPE_TIME(_name_) _coScopeTime coCONCAT(_scopeProfile, __LINE__)(_name_); coPROFILE_EVENT(_name_)