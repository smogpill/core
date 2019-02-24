// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "prebuild/pch.h"
#include "prebuild/generator/plugins/coMainEntryPointGeneratorPlugin.h"
#include "prebuild/generator/plugins/coCppGeneratorUtils.h"
#include "prebuild/generator/coProjectGenerator.h"
#include "lang/result/coResult_f.h"
#include "io/path/coPath_f.h"
#include "io/dir/coDirectory_f.h"
#include "io/stream/coStringOutputStream.h"
#include "io/file/coFileAccess.h"
#include "memory/allocator/coLocalAllocator.h"

coResult coMainEntryPointGeneratorPlugin::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	return true;
}

coResult coMainEntryPointGeneratorPlugin::Generate(const coParsedProject& _parsedProject)
{
	coTRY(Super::Generate(_parsedProject), nullptr);

	const coConstString& outDir = projectGenerator->GetOutReferenceDir();
	coJoinPaths(projectGenDir, outDir, projectGenerator->GetProjectRelativePath());
	coTRY(coCreateDirsIfMissing(projectGenDir), "Failed to create: " << projectGenDir);

	coTRY(GenerateCpp(), "Failed to generate the main cpp entry point");

	return true;
}

coResult coMainEntryPointGeneratorPlugin::GenerateCpp()
{
	coLocalAllocator localAllocator(4048);

	// Paths
	coDynamicString cxxPath(localAllocator);
	{
		coJoinPaths(cxxPath, projectGenDir, "allGen.cxx");
		coASSERT(coIsPathNormalized(cxxPath));
	}

	coStringOutputStream stream;
	coWriteHeader(stream);
	for (const coDynamicString* path : projectGenerator->GetGeneratedEntryPaths())
	{
		coASSERT(path);
		coConstString ext;
		coGetExtension(ext, *path);
		if (ext == ".cxx")
		{
			coWriteInclude(stream, *path);
		}
	}

	coTRY(stream.GetResult(), "Failed to write to stream: " << stream);

	coFileAccess file;
	{
		coFileAccess::InitConfig c;
		c.path = cxxPath;
		c.mode = coFileAccess::write;
		coTRY(file.Init(c), "Failed to open for writing: " << file);
	}

	coDynamicArray<coByte> output;
	stream.GetOutput(output);
	coTRY(file.Write(output), "Failed to write in: " << file);

	return true;
}
