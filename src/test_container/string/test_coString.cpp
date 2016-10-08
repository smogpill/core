// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_container/pch.h"
#include "test/unit/coTest.h"
#include "container/string/coString.h"
#include "container/string/coString_f.h"
#include "container/string/coConstString.h"
#include "container/string/coDynamicString.h"

coTEST(coString, coRemoveMultiples)
{
	const coConstString a[][2] =
	{
		{"", ""},
		{"-", "-"},
		{"--", "-"},
		{"---", "-"},
		{"-a-", "-a-"},
		{"--a--", "-a-"},
		{"aa-", "aa-"},
		{"aa--", "aa-"}
	};
	for (const auto& e : a)
	{
		coDynamicString s(e[0]);
		coRemoveMultiples(s, '-');
		coEXPECT(s == e[1]);
	}
}
