// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "parser/pch.h"
#include "parser/source/coSourceParser.h"

coSourceParser::ParseConfig::ParseConfig()
	: precompiledHeader(false)
{

}

coSourceParser::ParseResult::ParseResult()
	: parsedTypes(nullptr)
{

}

coResult coSourceParser::Parse(ParseResult& /*_result*/, const ParseConfig& /*_config*/)
{
	return true;
}

coResult coSourceParser::ParsePrecompiledHeader(const ParseConfig& /*_config*/)
{
	return true;
}
