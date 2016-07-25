// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "prebuild/pch.h"
#include "prebuild/coAppImpl.h"
#include "lang/result/coResult_f.h"
#include "io/path/coPathStatus.h"
#include "io/path/coPath_f.h"
#include "app/coCommandLineArgs.h"
#include "app/coProject_f.h"
#include "parser/project/coParsedProject.h"

coAppImpl::coAppImpl()
{

}

coAppImpl::~coAppImpl()
{
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

	{
		coCommandLineArgs::ArgConfig c;
		c.name = "outputDir";
		coTRY(argParser.Add(c), nullptr);
	}
	
	coTRY(argParser.Parse(_config.argv, _config.nbArgs), nullptr);

	// Project dir
	{
		projectDir = argParser.GetArgValue("projectDir");
		coNormalizePath(projectDir);
		coTRY(projectDir.count, "Project path is null");
		coTRY(coIsDirectory(projectDir), "Path is not a directory: " << projectDir);
	}

	// Output dir
	{
		outputDir = argParser.GetArgValue("outputDir");
		coNormalizePath(outputDir);
		coTRY(outputDir.count, "The output dir is null");
	}

	return true;
}

coResult coAppImpl::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);

	coTRY(ParseArgs(config), "Failed to parse args");

	{
		coProjectParser::InitConfig c;
		coTRY(projectParser.Init(c), nullptr);
	}

	return true;
}

coResult coAppImpl::OnStart()
{
	coProjectParser::ParseConfig parseConfig;
	parseConfig.projectDir = projectDir;
	parseConfig.outDir = outputDir;
	parseConfig.precompiledHeaderRelativePath = "pch.h";
	coParsedProject parsedProject;
	coTRY(projectParser.Parse(parsedProject, parseConfig), "Failed to parse the project: "<<projectDir);
	
	return true;
}
