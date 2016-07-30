// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "prebuild/pch.h"
#include "prebuild/generator/plugins/coMainEntryPointGeneratorPlugin.h"
#include "prebuild/generator/plugins/coCppGeneratorUtils.h"
#include "prebuild/generator/coProjectGenerator.h"
#include "lang/result/coResult_f.h"
#include "io/path/coPath_f.h"
#include "io/dir/coDirectory_f.h"
#include "io/file/coFileStreamBuffer.h"
#include "io/stream/coStringOutputStream.h"
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

	coFileStreamBuffer streamBuffer;
	{
		coFileStreamBuffer::InitConfig c;
		c.path = cxxPath;
		c.mode = coFileStreamBuffer::write;
		coTRY(streamBuffer.Init(c), "Failed to open for writing: " << streamBuffer);
	}

	coStringOutputStream stream;
	{
		coStringOutputStream::InitConfig c;
		c.buffer = &streamBuffer;
		coTRY(stream.Init(c), nullptr);
	}

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

	stream.Flush();
	coTRY(stream.GetResult(), "Failed to write to stream: " << stream);

	return true;
}
