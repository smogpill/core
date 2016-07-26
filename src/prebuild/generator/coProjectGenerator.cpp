// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "prebuild/pch.h"
#include "prebuild/generator/coProjectGenerator.h"
#include "prebuild/generator/coProjectGeneratorPlugin.h"
#include "lang/result/coResult_f.h"
#include "io/path/coPath_f.h"
#include "io/dir/coDirectory_f.h"

coProjectGenerator::InitConfig::InitConfig()
{
}

coResult coProjectGenerator::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);

	plugins = config.plugins;
	for (coProjectGeneratorPlugin* plugin : plugins)
	{
		coASSERT(plugin->IsInitialized());
		plugin->_SetProjectGenerator(*this);
	}

	// Check project dir
	projectDir = config.projectDir;
	coTRY(coIsPathNormalized(projectDir), nullptr);
	coTRY(coIsDirectory(projectDir), "Not a dir: " << projectDir);

	// Check out dir
	outDir = config.outDir;
	coTRY(coIsPathNormalized(outDir), nullptr);
	coTRY(coCreateDirsIfMissing(outDir), "Failed to create the output directory: " << outDir);

	// Check precompiled header
	precompiledHeaderRelativePath = config.precompiledHeaderRelativePath;
// 	if (config.precompiledHeaderRelativePath != "")
// 	{
// 		coTRY(coIsPathNormalized(config.precompiledHeaderRelativePath), nullptr);
// 		coTRY(coIsFile(config.precompiledHeaderRelativePath), "Not a file: "<<config.precompiledHeaderRelativePath);
// 	}

	return true;
}

coResult coProjectGenerator::Generate(const coParsedProject& _parsedProject)
{
	coASSERT(IsInitialized());

	for (coProjectGeneratorPlugin* plugin : plugins)
	{
		coTRY(plugin->Generate(_parsedProject), "Failed to generate using the plugin: "<<plugin->GetDebugName());
	}
	return true;
}
