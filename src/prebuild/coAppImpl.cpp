// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "prebuild/pch.h"
#include "prebuild/coAppImpl.h"
#include "lang/result/coResult_f.h"
#include "parser/reflect/coReflectParser.h"

coAppImpl::coAppImpl()
	: parser(nullptr)
{

}

coAppImpl::~coAppImpl()
{
	delete parser, parser = nullptr;
}

coResult coAppImpl::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	//const InitConfig& config = static_cast<const InitConfig&>(_config);

	coTRY(!parser, nullptr);
	parser = coReflectParser::Create();

	return true;
}

coResult coAppImpl::OnStart()
{
	coTRY(parser, nullptr);

	{
		coReflectParser::InitConfig config;
		coTRY(parser->Init(config), nullptr);
	}
	
	{
		coReflectParser::ParseConfig config;
		config.filePath = "J:/CODE/core/src/pattern/object/coObject.h";
		coTRY(parser->Parse(config), nullptr);
	}
	
	return true;
}
