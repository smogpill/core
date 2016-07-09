// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coApp : public coObject
{
	typedef coObject Super;
public:
	class InitConfig : public Super::InitConfig
	{
	public:
		coConstArray<coConstString> args;
	};
};
