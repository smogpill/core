// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "prebuild/pch.h"
#include "prebuild/generator/cpp/coCppReflectGeneratorPlugin.h"
#include "prebuild/generator/coProjectGenerator.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coType.h"
#include "io/path/coPath_f.h"
#include "io/file/coFileStreamBuffer.h"
#include "io/dir/coDirectory_f.h"
#include "io/stream/coStringOutputStream.h"
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
	coLocalAllocator localAllocator(4048);

	// Paths
	coDynamicString cppPath(localAllocator);
	{
		coJoinPaths(cppPath, genDir, "allTypes.cpp");
		coASSERT(coIsPathNormalized(cppPath));
	}

	coFileStreamBuffer streamBuffer;
	{
		coFileStreamBuffer::InitConfig c;
		c.path = cppPath;
		c.mode = coFileStreamBuffer::write;
		coTRY(streamBuffer.Init(c), "Failed to open for writing: " << streamBuffer.GetDebugName());
	}

	coStringOutputStream stream;
	{
		coStringOutputStream::InitConfig c;
		c.buffer = &streamBuffer;
		coTRY(stream.Init(c), nullptr);
	}

	stream << "// Generated\n";
	stream << "#include \"" << projectGenerator->GetPrecompiledHeaderRelativepath() << "\"\n";
	stream << "\n";

	coDynamicString cxxPath(localAllocator);

	for (const coParsedType* parsedType : _parsedProject.parsedTypes)
	{
		coASSERT(parsedType);
		coTRY(GenerateType(cxxPath, *parsedType), "Failed to generate type: " << parsedType->GetDebugName());
		stream << "#include \"" << cxxPath << "\"\n";
	}

	stream.Flush();
	coTRY(stream.GetResult(), "Failed to write to stream: " << stream.GetDebugName());

	return true;
}

coResult coCppReflectGeneratorPlugin::GenerateType(coDynamicString& _outPath, const coParsedType& _parsedType)
{
	const coType* type = _parsedType.type;
	coASSERT(type);
	coTRY(type->name != "", "Empty type name");

	coLocalAllocator localAllocator(4048);

	// Paths
	{
		coDynamicString commonPath(localAllocator);
		coJoinPaths(commonPath, genDir, type->name);

		// Cxx path
		_outPath << commonPath << ".cxx";
		coASSERT(coIsPathNormalized(_outPath));
	}

	coFileStreamBuffer streamBuffer;
	{
		coFileStreamBuffer::InitConfig c;
		c.path = _outPath;
		c.mode = coFileStreamBuffer::write;
		coTRY(streamBuffer.Init(c), "Failed to open for writing: " << streamBuffer.GetDebugName());
	}

	coStringOutputStream stream;
	{
		coStringOutputStream::InitConfig c;
		c.buffer = &streamBuffer;
		coTRY(stream.Init(c), nullptr);
	}

	stream << "// Generated\n";
	stream << "#include \"" << _parsedType.sourcePath << "\"\n";

	stream.Flush();
	coTRY(stream.GetResult(), "Failed to write to stream: "<<stream.GetDebugName());

	return true;
}
