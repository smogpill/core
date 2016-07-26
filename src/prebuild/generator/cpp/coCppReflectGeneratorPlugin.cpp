// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "prebuild/pch.h"
#include "prebuild/generator/cpp/coCppReflectGeneratorPlugin.h"
#include "prebuild/generator/coProjectGenerator.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coType.h"
#include "io/path/coPath_f.h"
#include "io/file/coFileAccess.h"
#include "io/dir/coDirectory_f.h"
#include "parser/project/coParsedProject.h"
#include "parser/source/coParsedType.h"
#include "memory/allocator/coLocalAllocator.h"

static const coConstString co_genPath = "gen";

coResult coCppReflectGeneratorPlugin::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	return true;
}

coResult coCppReflectGeneratorPlugin::Generate(const coParsedProject& _parsedProject)
{
	coTRY(Super::Generate(_parsedProject), nullptr);

	const coConstString& outDir = projectGenerator->GetOutDir();
	coTRY(coIsDirectory(outDir), nullptr);
	coJoinPaths(genDir, outDir, co_genPath);
	coASSERT(coIsPathNormalized(genDir));
	coTRY(coCreateDirsIfMissing(genDir), "Failed to create: " << genDir);

	coTRY(GenerateTypes(_parsedProject), "Failed to generate types.");

	return true;
}

coResult coCppReflectGeneratorPlugin::GenerateTypes(const coParsedProject& _parsedProject)
{
	for (const coParsedType* parsedType : _parsedProject.parsedTypes)
	{
		coASSERT(parsedType);
		coTRY(GenerateType(*parsedType), "Failed to generate type: " << parsedType->GetDebugName());
	}
	return true;
}

coResult coCppReflectGeneratorPlugin::GenerateType(const coParsedType& _parsedType)
{
	const coType* type = _parsedType.type;
	coASSERT(type);
	coTRY(type->name != "", "Empty type name");

	coLocalAllocator localAllocator(4048);

	// Paths
	coDynamicString cxxPath(localAllocator);
	{
		coDynamicString commonPath(localAllocator);
		coJoinPaths(commonPath, genDir, type->name);

		// Cxx path
		cxxPath << commonPath << ".cxx";
		coASSERT(coIsPathNormalized(cxxPath));
	}

	coFileAccess cxxAccess;
	{
		coFileAccess::InitConfig config;
		config.mode = coFileAccess::write;
		config.path = cxxPath;
		coTRY(cxxAccess.Init(config), "Failed to open for writing: "<<cxxAccess.GetDebugName());
	}

	return true;
}
