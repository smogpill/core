// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "prebuild/pch.h"
#include "prebuild/generator/cpp/coCppReflectGeneratorPlugin.h"
#include "prebuild/generator/coProjectGenerator.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coType.h"
#include "lang/reflect/coField.h"
#include "io/path/coPath_f.h"
#include "io/file/coFileStreamBuffer.h"
#include "io/dir/coDirectory_f.h"
#include "io/stream/coStringOutputStream.h"
#include "parser/project/coParsedProject.h"
#include "parser/source/coParsedType.h"
#include "parser/source/coParsedField.h"
#include "memory/allocator/coLocalAllocator.h"

static const coConstString co_genPath = "gen";
static const coConstString co_typesPath = "types";

coResult coCppReflectGeneratorPlugin::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	return true;
}

coResult coCppReflectGeneratorPlugin::Generate(const coParsedProject& _parsedProject)
{
	coTRY(Super::Generate(_parsedProject), nullptr);

	const coConstString& outDir = projectGenerator->GetOutReferenceDir();
	const coConstString& projectPath = projectGenerator->GetProjectRelativePath();
	coJoinPaths(projectGenDir, outDir, projectPath);
	coASSERT(coIsPathNormalized(projectGenDir));
	coTRY(coCreateDirsIfMissing(projectGenDir), "Failed to create: " << projectGenDir);

	coJoinPaths(typesGenDir, projectGenDir, co_typesPath);
	coASSERT(coIsPathNormalized(typesGenDir));
	coTRY(coCreateDirsIfMissing(typesGenDir), "Failed to create: " << typesGenDir);

	coTRY(GenerateTypes(_parsedProject), "Failed to generate types.");

	return true;
}

coResult coCppReflectGeneratorPlugin::GenerateTypes(const coParsedProject& _parsedProject)
{
	coLocalAllocator localAllocator(4048);

	// Paths
	coDynamicString cppPath(localAllocator);
	{
		coJoinPaths(cppPath, projectGenDir, "allTypes.cpp");
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
	stream << "#include \"" << projectGenerator->GetPrecompiledHeaderRelativePath() << "\"\n";
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

coResult coCppReflectGeneratorPlugin::GenerateType(coDynamicString& _relativePath, const coParsedType& _parsedType)
{
	const coType* type = _parsedType.type;
	coASSERT(type);
	coTRY(type->name != "", "Empty type name");

	coLocalAllocator localAllocator(4048);
	coDynamicString absolutePath(localAllocator);

	// Paths
	{
		const coConstString& projectPath = projectGenerator->GetProjectRelativePath();
		coDynamicString tmpPath(localAllocator);
		coJoinPaths(tmpPath, projectPath, co_typesPath);
		coJoinPaths(_relativePath, tmpPath, type->name);
		_relativePath << ".cxx";

		const coConstString& outPath = projectGenerator->GetOutReferenceDir();
		coJoinPaths(absolutePath, outPath, _relativePath);
		coASSERT(coIsPathNormalized(_relativePath));
	}

	coFileStreamBuffer streamBuffer;
	{
		coFileStreamBuffer::InitConfig c;
		c.path = absolutePath;
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
	stream << "#include \"lang/reflect/coType.h\"\n";
	if (_parsedType.parsedFields.count)
	{
		stream << "#include \"lang/reflect/coField.h\"\n";
	}
	stream << "\n";

	stream << "coType* co_CreateType()\n";
	stream << "{\n";
	
	coTRY(WriteParsedType(stream, _parsedType, "\t"), "Failed to write type: "<<_parsedType.GetDebugName());

	stream << "\treturn type;\n";
	stream << "}\n";

	stream.Flush();
	coTRY(stream.GetResult(), "Failed to write to stream: "<<stream.GetDebugName());

	return true;
}

coResult coCppReflectGeneratorPlugin::WriteSymbol(coStringOutputStream& _stream, const coSymbol& _symbol, const coConstString& _indentation, const coConstString& _prefix)
{
	_stream << _indentation << _prefix << "id = 0;\n";
	_stream << _indentation << _prefix << "name = \"" << _symbol.name << "\";\n";
	_stream << _indentation << _prefix << "symbolFlags = " << _symbol.symbolFlags << ";\n";
	return true;
}

coResult coCppReflectGeneratorPlugin::WriteParsedType(coStringOutputStream& _stream, const coParsedType& _parsedType, const coConstString& _indentation)
{
	const coType* type = _parsedType.type;
	coASSERT(type);

	_stream << _indentation << "coType* type = new coType();\n";
	coTRY(WriteSymbol(_stream, *type, "\t", "type->"), nullptr);
	_stream << _indentation << "type->size8 = sizeof(" << type->name << ");\n";
	_stream << _indentation << "type->super = nullptr;\n";

	_stream << _indentation << "\n";

	// Fields
	if (_parsedType.parsedFields.count)
	{
		_stream << _indentation << "// Fields\n";
		_stream << _indentation << "coReserve(type->fields, " << _parsedType.parsedFields.count << ");\n";
		_stream << _indentation << "\n";
		for (const coParsedField* parsedField : _parsedType.parsedFields)
		{
			coASSERT(parsedField);
			coTRY(WriteParsedField(_stream, *parsedField, _indentation), "Failed to write field: "<<parsedField->field->name);
		}
	}

	return true;
}

coResult coCppReflectGeneratorPlugin::WriteParsedField(coStringOutputStream& _stream, const coParsedField& _parsedField, const coConstString& _indentation)
{
	coLocalAllocator localAllocator(2048);

	const coField* field = _parsedField.field;
	coASSERT(field);
	_stream << _indentation << "// " << field->name << "\n";
	_stream << _indentation << "{\n";

	coDynamicString blockIndent(localAllocator);
	blockIndent << _indentation << "\t";
	_stream << blockIndent << "coField* field = new coField();\n";
	coTRY(WriteSymbol(_stream, *field, blockIndent, "field->"), nullptr);
	_stream << blockIndent << "field->type = nullptr;\n";

	_stream << blockIndent << "\n";
	_stream << blockIndent << "coPushBack(type->fields, field);\n";
	_stream << _indentation << "}\n";
	_stream << _indentation << "\n";
	coASSERT(field);
	return true;
}