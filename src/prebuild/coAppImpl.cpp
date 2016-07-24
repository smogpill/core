// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "prebuild/pch.h"
#include "prebuild/coAppImpl.h"
#include "lang/result/coResult_f.h"
#include "io/path/coPathStatus.h"
#include "io/path/coPath_f.h"
#include "app/coCommandLineArgs.h"
#include "app/coProject_f.h"

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
		const coConstString& dir = argParser.GetArgValue("projectDir");
		coTRY(dir.count, "Project path is null");
		coPathStatus pathStatus;
		coTRY(coGetPathStatus(pathStatus, dir), "Failed to get the path status: " << dir);
		coTRY(pathStatus.Exists(), "Path does not exist: " << dir);
		coTRY(pathStatus.IsDirectory(), "Path is not a directory: " << dir);
		projectDir = dir;
	}

	// Output dir
	{
		const coConstString& dir = argParser.GetArgValue("outputDir");
		coTRY(dir.count, "The output dir is null");
		/*coPathStatus pathStatus;
		coTRY(coGetPathStatus(pathStatus, dir), "Failed to get the path status: " << dir);
		coTRY(pathStatus.Exists(), "Path does not exist: " << dir);
		coTRY(pathStatus.IsDirectory(), "Path is not a directory: " << dir);*/
		outputDir = dir;
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
	coTRY(projectParser.Parse(parseConfig), "Failed to parse the project: "<<projectDir);
	
	return true;
}
