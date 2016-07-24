// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "prebuild/pch.h"
#include "prebuild/coAppImpl.h"
#include "lang/result/coResult_f.h"
#include "parser/reflect/coReflectParser.h"
#include "io/path/coPathStatus.h"
#include "io/path/coPath_f.h"
#include "app/coCommandLineArgs.h"
#include "app/coProject_f.h"

coAppImpl::coAppImpl()
	: parser(nullptr)
{

}

coAppImpl::~coAppImpl()
{
	delete parser, parser = nullptr;
}

coResult coAppImpl::ParseArgs(const InitConfig& _config)
{
	coCommandLineArgs argParser;

	{
		coCommandLineArgs::InitConfig c;
		c.commandName = coPROJECT_NAME_WITH_VERSION_STRING;
		coTRY(argParser.Init(c), nullptr);
	}

	{
		coCommandLineArgs::ArgConfig c;
		c.name = "projectDir";
		coTRY(argParser.Add(c), nullptr);
	}
	
	coTRY(argParser.Parse(_config.argv, _config.nbArgs), nullptr);
	projectDir = argParser.GetArgValue("projectDir");

	return true;
}

coResult coAppImpl::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);

	coTRY(ParseArgs(config), "Failed to parse args");

	coTRY(projectDir.count, "Project path is empty");

	coPathStatus pathStatus;
	coTRY(coGetPathStatus(pathStatus, projectDir), "Failed to get the path status: " << projectDir);
	coTRY(pathStatus.Exists(), "Path does not exist: " << projectDir);

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
