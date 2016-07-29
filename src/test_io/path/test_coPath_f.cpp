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
	coDynamicString p;

	p = "c:\\chicken\\\\small.jpeg";
	coNormalizePath(p);
	coEXPECT(p == "c:/chicken/small.jpeg");
	coEXPECT(coIsPathNormalized(p));

	p = "/";
	coNormalizePath(p);
	coEXPECT(p == "/");
	coEXPECT(coIsPathNormalized(p));

	p = "./";
	coNormalizePath(p);
	coEXPECT(p == ".");
	coEXPECT(coIsPathNormalized(p));

	p = ".//";
	coNormalizePath(p);
	coEXPECT(p == ".");
	coEXPECT(coIsPathNormalized(p));

	p = "c:/chicken.jpeg";
	coNormalizePath(p);
	coEXPECT(p == "c:/chicken.jpeg");
	coEXPECT(coIsPathNormalized(p));

	p = "./chicken.jpeg";
	coNormalizePath(p);
	coEXPECT(p == "chicken.jpeg");
	coEXPECT(coIsPathNormalized(p));

	p = "c:/chicken_burger.jpeg";
	coNormalizePath(p);
	coEXPECT(p == "c:/chicken_burger.jpeg");
	coEXPECT(coIsPathNormalized(p));
}

coTEST(coPath_f, coGetFileName)
{
	coConstString s;

	coGetFileName(s, "c:/chicken.jpeg");
	coEXPECT(s == "chicken.jpeg");

	coGetFileName(s, "c:/chicken.small.jpeg");
	coEXPECT(s == "chicken.small.jpeg");

	coGetFileName(s, ".");
	coEXPECT(s == ".");

	coGetFileName(s, "..");
	coEXPECT(s == "..");

	coGetFileName(s, "c:/chicken-small.jpeg");
	coEXPECT(s == "chicken-small.jpeg");
}

coTEST(coPath_f, coGetExtension)
{
	coConstString s;

	coGetExtension(s, "c:/chicken.jpeg");
	coEXPECT(s == ".jpeg");
	
	coGetExtension(s, "c:/chicken.small.jpeg");
	coEXPECT(s == ".jpeg");
	
	coGetExtension(s, ".");
	coEXPECT(s == "");
	
	coGetExtension(s, "..");
	coEXPECT(s == "");

	coGetExtension(s, "c:/chicken-small.jpeg");
	coEXPECT(s == ".jpeg");

	coGetExtension(s, "c:/hey.zer/chicken-small.jpeg");
	coEXPECT(s == ".jpeg");

	coGetExtension(s, "c:/hey.zer/chicken-small");
	coEXPECT(s == "");
}

coTEST(coPath_f, coGetBaseName)
{
	coConstString s;

	coGetBaseName(s, "c:/chicken.jpeg");
	coEXPECT(s == "chicken");

	coGetBaseName(s, "c:/chicken.small.jpeg");
	coEXPECT(s == "chicken.small");

	coGetBaseName(s, ".");
	coEXPECT(s == "");

	coGetBaseName(s, "..");
	coEXPECT(s == "");

	coGetBaseName(s, "c:/chicken-small.jpeg");
	coEXPECT(s == "chicken-small");
}

coTEST(coPath_f, coJoinPaths)
{
	coDynamicString s;

	coJoinPaths(s, nullptr, nullptr);
	coEXPECT(s == "");

	coJoinPaths(s, "chicken", nullptr);
	coEXPECT(s == "chicken");

	coJoinPaths(s, nullptr, "chicken");
	coEXPECT(s == "chicken");

	coJoinPaths(s, "chi", "cken");
	coEXPECT(s == "chi/cken");

	coJoinPaths(s, "", "");
	coEXPECT(s == "");

	coJoinPaths(s, "c:/hep", "chicken");
	coEXPECT(s == "c:/hep/chicken");

	coJoinPaths(s, ".", "chicken.png");
	coEXPECT(s == "chicken.png");

	coJoinPaths(s, ".", nullptr);
	coEXPECT(s == ".");
}
