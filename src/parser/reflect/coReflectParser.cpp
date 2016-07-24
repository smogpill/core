// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "parser/pch.h"
#include "parser/reflect/coReflectParser.h"

coReflectParser::ParseConfig::ParseConfig()
	: precompiledHeader(false)
{

}

coResult coReflectParser::Parse(const ParseConfig& /*_config*/)
{
	return true;
}

coResult coReflectParser::ParsePrecompiledHeader(const ParseConfig& /*_config*/)
{
	return true;
}
