// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "prebuild/pch.h"
#include "prebuild/coAppImpl.h"
#include "prebuild/generator/coProjectGenerator.h"
#include "prebuild/generator/cpp/coCppReflectGeneratorPlugin.h"
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
		c.name = "srcReferenceDir";
		coTRY(argParser.Add(c), nullptr);
	}

	{
		coCommandLineArgs::ArgConfig c;
		c.name = "outReferenceDir";
		coTRY(argParser.Add(c), nullptr);
	}

	{
		coCommandLineArgs::ArgConfig c;
		c.name = "projectRelativePath";
		coTRY(argParser.Add(c), nullptr);
	}

	{
		coCommandLineArgs::ArgConfig c;
		c.name = "pchRelativePath";
		//c.optional = true;
		coTRY(argParser.Add(c), nullptr);
	}
	
	coTRY(argParser.Parse(_config.argv, _config.nbArgs), nullptr);

	// Src reference dir
	{
		srcReferenceDir = argParser.GetArgValue("srcReferenceDir");
		coNormalizePath(srcReferenceDir);
		coTRY(srcReferenceDir.count, "Src reference dir is null");
		coTRY(coIsDirectory(srcReferenceDir), "Path is not a directory: " << srcReferenceDir);
	}

	// Out reference dir
	{
		outReferenceDir = argParser.GetArgValue("outReferenceDir");
		coNormalizePath(outReferenceDir);
		coTRY(outReferenceDir.count, "The out reference dir is null");
	}

	// Project path
	{
		projectRelativePath = argParser.GetArgValue("projectRelativePath");
		coNormalizePath(projectRelativePath);
		coTRY(projectRelativePath.count, "The project path is null");
	}

	// Pch path
	{
		pchRelativePath = argParser.GetArgValue("pchRelativePath");
		if (pchRelativePath != "")
		{
			coNormalizePath(pchRelativePath);
		}
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
	coParsedProject parsedProject;
	coINFO("Parsing project...");
	coTRY(ParseProject(parsedProject), "Failed to parse the project: "<<projectRelativePath);
	coINFO("Parsing project done.");
	coINFO("Generating source files...");
	coTRY(GenerateProject(parsedProject), "Failed to generate the project: " << projectRelativePath);
	coINFO("Generating source files done.");

	return true;
}

coResult coAppImpl::ParseProject(coParsedProject& _parsedProject)
{
	coProjectParser::ParseConfig parseConfig;
	parseConfig.srcReferenceDir = srcReferenceDir;
	parseConfig.outReferenceDir = outReferenceDir;
	parseConfig.projectRelativePath = projectRelativePath;
	parseConfig.precompiledHeaderPath = pchRelativePath;
	coTRY(projectParser.Parse(_parsedProject, parseConfig), "Failed to parse the project: " << projectRelativePath);

	return true;
}

coResult coAppImpl::GenerateProject(const coParsedProject& _parsedProject)
{
	coCppReflectGeneratorPlugin cppReflectPlugin;
	{
		coCppReflectGeneratorPlugin::InitConfig c;
		coTRY(cppReflectPlugin.Init(c), "Failed to init the plugin: "<<cppReflectPlugin.GetDebugName());
	}

	coProjectGenerator projectGenerator;
	{
		coHACK("hard coded paths");
		coProjectGenerator::InitConfig c;
		c.outReferenceDir = outReferenceDir;
		c.projectRelativePath = projectRelativePath;
		c.srcReferenceDir = srcReferenceDir;
		c.precompiledHeaderRelativePath = pchRelativePath;
		/*c.projectDir = projectDir;
		c.outDir = outputDir;
		c.precompiledHeaderPath = co_pchPath;*/
		c.plugins = {&cppReflectPlugin};
		coTRY(projectGenerator.Init(c), "Failed to init the project generator.");
	}
	
	coTRY(projectGenerator.Generate(_parsedProject), nullptr);
	return true;
}
