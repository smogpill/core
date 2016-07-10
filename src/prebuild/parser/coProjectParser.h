// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"
#include "container/string/coConstString.h"

class coProjectParser : public coObject
{
public:
	struct ParseConfig
	{
		ParseConfig();

		coConstString projectDir;
	};

	coResult Parse(const ParseConfig& _config);
};
