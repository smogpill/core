// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/string/coConstString.h>

class _coScopeProfile
{
public:
	_coScopeProfile(const coConstString& name);
	~_coScopeProfile();

private:
	coUint64 startTime;
	const coConstString name;
};

#define coSCOPE_PROFILE(_name_) _coScopeProfile coCONCAT(_scopeProfile, __LINE__)(_name_)