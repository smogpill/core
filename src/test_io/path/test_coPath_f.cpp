// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_io/pch.h"
#include "test/unit/coTest.h"
#include "io/path/coPath_f.h"
#include "container/string/coConstString.h"

coTEST(coPath_f, coIsDotOrDoubleDot)
{
	coEXPECT(!coIsDotOrDoubleDot("qsfsfq"));
	coEXPECT(coIsDotOrDoubleDot("."));
	coEXPECT(coIsDotOrDoubleDot(".."));
	coEXPECT(!coIsDotOrDoubleDot("./"));
	coEXPECT(!coIsDotOrDoubleDot("../"));
	coEXPECT(!coIsDotOrDoubleDot("..."));
	coEXPECT(!coIsDotOrDoubleDot("./a"));
	coEXPECT(!coIsDotOrDoubleDot("../a"));
	coEXPECT(!coIsDotOrDoubleDot("a/."));
	coEXPECT(!coIsDotOrDoubleDot("a/.."));
	coEXPECT(!coIsDotOrDoubleDot("a"));
}

coTEST(coPath_f, coIsDotHiddenPath)
{
	coEXPECT(coIsDotHiddenPath(".chicken"));
	coEXPECT(coIsDotHiddenPath(".chicken/small"));
	coEXPECT(!coIsDotHiddenPath("."));
	coEXPECT(!coIsDotHiddenPath(".."));
	coEXPECT(!coIsDotHiddenPath("..chicken"));
	coEXPECT(!coIsDotHiddenPath("chicken/.small"));
	coEXPECT(!coIsDotHiddenPath("./hey.cpp"));
}

coTEST(coPath_f, coNormalizePath)
{
	const coConstString a[][2] = 
	{
		{ "/", "/" },
		{ "//", "//" },
		{ "///", "/" },
		{ "./", "." },
		{ "./a", "a" },
		{ "./a.b", "a.b" },
		{ ".//", "." },
		{ ".///", "." },
		{ "..", ".." },
		{ "../", ".." },
		{ "../a", "../a" },
		{ "../a.b", "../a.b" },
		{ ".a", ".a" },
		{ "./.a", ".a" },
		{ "c:/a.c", "c:/a.c" },
		{ "./a.c", "a.c" },
		{ "a.b/c/", "a.b/c" },
		{ "c:\\a\\\\b.c", "c:/a/b.c" },

// 		{ "c:/a/./b.c", "c:/a/b.c" },
// 		{ "c:/a/././b.c", "c:/a/b.c" },
// 		{ "c:/a/../b.c", "c:/b.c" },
// 		{ "c:\\\\chicken\\..\\sandwich\\/\\chicken.jpeg", "c:/sandwich/chicken.jpeg" }
	};
	coDynamicString p;
	for (const auto& e : a)
	{
		p = e[0];
		coNormalizePath(p);
		coEXPECT(p == e[1]);
		coEXPECT(coIsPathNormalized(p));
	}
}

coTEST(coPath_f, coGetFileName)
{
	const coConstString a[][2] =
	{
		{ "c:/a.b", "a.b" },
		{ "c:/a.b.c", "a.b.c" },
		{ ".", "." },
		{ "..", ".." },
		{ "c:/a-b.c", "a-b.c" },
	};
	for (const auto& e : a)
	{
		coConstString s;
		coGetFileName(s, e[0]);
		coEXPECT(s == e[1]);
	}
}

coTEST(coPath_f, coGetExtension)
{
	const coConstString a[][2] =
	{
		{ "c:/chicken.jpeg", ".jpeg" },
		{ "c:/chicken.small.jpeg", ".jpeg" },
		{ ".", "" },
		{ "..", "" },
		{ "c:/chicken-small.jpeg", ".jpeg" },
		{ "c:/hey.zer/chicken-small.jpeg", ".jpeg" },
		{ "c:/hey.zer/chicken-small", "" },
	};
	for (const auto& e : a)
	{
		coConstString s;
		coGetExtension(s, e[0]);
		coEXPECT(s == e[1]);
	}
}

coTEST(coPath_f, coGetBaseName)
{
	const coConstString a[][2] =
	{
		{ "c:/chicken.jpeg", "chicken" },
		{ "c:/chicken.small.jpeg", "chicken.small" },
		{ ".", "" },
		{ "..", "" },
		{ "c:/chicken-small.jpeg", "chicken-small" },
	};
	for (const auto& e : a)
	{
		coConstString s;
		coGetBaseName(s, e[0]);
		coEXPECT(s == e[1]);
	}
}

coTEST(coPath_f, coJoinPaths)
{
	const coConstString a[][3] =
	{
		{ nullptr, nullptr, "" },
		{ "chicken", nullptr, "chicken" },
		{ nullptr, "chicken", "chicken" },
		{ "chi", "cken", "chi/cken" },
		{ "", "", "" },
		{ "c:/hep", "chicken", "c:/hep/chicken" },
		{ ".", "chicken.png", "chicken.png" },
		{ ".", nullptr, "." },
	};
	coDynamicString s;
	for (const auto& e : a)
	{
		coJoinPaths(s, e[0], e[1]);
		coEXPECT(s == e[2]);
	}
}
