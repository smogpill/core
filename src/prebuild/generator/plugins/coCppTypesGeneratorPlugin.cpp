// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "prebuild/pch.h"
#include "prebuild/generator/plugins/coCppTypesGeneratorPlugin.h"
#include "prebuild/generator/plugins/coCppGeneratorUtils.h"
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

coResult coCppTypesGeneratorPlugin::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	return true;
}

coResult coCppTypesGeneratorPlugin::Generate(const coParsedProject& _parsedProject)
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

coResult coCppTypesGeneratorPlugin::GenerateTypes(const coParsedProject& _parsedProject)
{
	coLocalAllocator localAllocator(4048);

	// Paths
	coDynamicString mainCxxAbsolutePath(localAllocator);
	coDynamicString mainCxxRelativePath(localAllocator);
	{
		coJoinPaths(mainCxxRelativePath, projectGenerator->GetProjectRelativePath(), "allTypes.cxx");
		coASSERT(coIsPathNormalized(mainCxxRelativePath));
		coJoinPaths(mainCxxAbsolutePath, projectGenerator->GetOutReferenceDir(), mainCxxRelativePath);
		coASSERT(coIsPathNormalized(mainCxxAbsolutePath));
	}

	coFileStreamBuffer streamBuffer;
	{
		coFileStreamBuffer::InitConfig c;
		c.path = mainCxxAbsolutePath;
		c.mode = coFileStreamBuffer::write;
		coTRY(streamBuffer.Init(c), "Failed to open for writing: " << streamBuffer.GetDebugName());
	}

	coStringOutputStream stream;
	{
		coStringOutputStream::InitConfig c;
		c.buffer = &streamBuffer;
		coTRY(stream.Init(c), nullptr);
	}

	coWriteHeader(stream);
	stream << "\n";

	coDynamicString cxxPath(localAllocator);

	for (const coParsedType* parsedType : _parsedProject.parsedTypes)
	{
		coASSERT(parsedType);
		coTRY(GenerateType(cxxPath, *parsedType), "Failed to generate type: " << parsedType->GetDebugName());
		coWriteInclude(stream, cxxPath);
	}

	stream.Flush();
	coTRY(stream.GetResult(), "Failed to write to stream: " << stream.GetDebugName());

	projectGenerator->AddGeneratedEntryPath(mainCxxRelativePath);

	return true;
}

coResult coCppTypesGeneratorPlugin::GenerateType(coDynamicString& _relativePath, const coParsedType& _parsedType)
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

	coWriteHeader(stream);
	coWriteInclude(stream, _parsedType.sourcePath);
	coWriteInclude(stream, "lang/reflect/coType.h");
	coWriteInclude(stream, "lang/reflect/coType_f.h");
	coWriteInclude(stream, "lang/result/coResult_f.h");
	coWriteInclude(stream, "lang/reflect/coTypeBuilder.h");
	coWriteInclude(stream, "lang/reflect/coTypeAutoRegistrator.h");
	if (_parsedType.parsedFields.count)
	{
		coWriteInclude(stream, "lang/reflect/coField.h");
		coWriteInclude(stream, "lang/reflect/coField_f.h");
	}
	coWriteInclude(stream, "lang/result/coResult.h");
	coWriteInclude(stream, "container/string/coDynamicString_f.h");
	stream << "\n";
	coTRY(WriteTypeBuilderDeclaration(stream, _parsedType), nullptr);
	coTRY(WriteInitTypeFunc(stream, _parsedType), nullptr);
	coTRY(WriteLinkTypeFunc(stream, _parsedType), nullptr);
	coTRY(WriteGetStaticTypeFunc(stream, _parsedType), nullptr);

	stream.Flush();
	coTRY(stream.GetResult(), "Failed to write to stream: "<<stream);

	return true;
}

coResult coCppTypesGeneratorPlugin::WriteInitTypeFunc(coStringOutputStream& _stream, const coParsedType& _parsedType)
{
	const coType* type = _parsedType.type;
	coASSERT(type);
	coASSERT(type->name != nullptr);
	_stream << "coResult " << type->name << "_typeBuilder::OnInitType()\n";
	_stream << "{\n";
	coTRY(WriteParsedType(_stream, _parsedType, "\t"), "Failed to write type: " << _parsedType.GetDebugName());
	_stream << "\treturn true;\n";
	_stream << "}\n";
	_stream << "\n";
	return true;
}

coResult coCppTypesGeneratorPlugin::WriteLinkTypeFunc(coStringOutputStream& _stream, const coParsedType& _parsedType)
{
	const coType* type = _parsedType.type;
	coASSERT(type);
	coASSERT(type->name != nullptr);
	_stream << "coResult " << type->name << "_typeBuilder::OnLinkType()\n";
	_stream << "{\n";
	if (_parsedType.superTypeName != "")
	{
		_stream << "\tcoTRY(type, nullptr);\n";
		_stream << "\ttype->super = coGetType<" << _parsedType.superTypeName << ">();\n";
		_stream << "\n";
	}
	coUint32 fieldIndex = 0;
	for (const coParsedField* parsedField : _parsedType.parsedFields)
	{
		const coField* field = parsedField->field;
		coASSERT(field);
		if (field->symbolFlags & coSymbol::public_)
		{
			coTRY(parsedField->typeName.count, nullptr);
			_stream << "\ttype->fields["<< fieldIndex<<"]->type = coGetType<" << parsedField->typeName << ">();\n";
			++fieldIndex;
		}
	}
	_stream << "\treturn true;\n";
	_stream << "}\n";
	_stream << "\n";
	return true;
}

coResult coCppTypesGeneratorPlugin::WriteTypeBuilderDeclaration(coStringOutputStream& _stream, const coParsedType& _parsedType)
{
	const coType* type = _parsedType.type;
	coASSERT(type);

	_stream << "class " << type->name << "_typeBuilder : public coTypeBuilder\n";
	_stream << "{\n";
	_stream << "public:\n";
	_stream << "\tvirtual coResult OnInitType() override;\n";
	_stream << "\tvirtual coResult OnLinkType() override;\n";
	_stream << "};\n";
	_stream << type->name << "_typeBuilder co_typeBuilder_" << type->name << ";\n";
	_stream << "coTypeAutoRegistrator co_typeAutoRegistrator_" << type->name << "(co_typeBuilder_" << type->name << ");\n";
	_stream << "\n";
	return true;
}

coResult coCppTypesGeneratorPlugin::WriteGetStaticTypeFunc(coStringOutputStream& _stream, const coParsedType& _parsedType)
{
	const coType* type = _parsedType.type;
	coASSERT(type);

	_stream << "const coType* " << type->name << "::GetStaticType()\n";
	_stream << "{\n";
	_stream << "\treturn co_typeBuilder_" << type->name << ".GetType();\n";
	_stream << "}\n";
	_stream << "\n";
	return true;
}

coResult coCppTypesGeneratorPlugin::WriteSymbol(coStringOutputStream& _stream, const coSymbol& _symbol, const coConstString& _indentation, const coConstString& _prefix)
{
	_stream << _indentation << _prefix << "name = \"" << _symbol.name << "\";\n";
	_stream << _indentation << _prefix << "nameHash = coHash32(" << _prefix << "name);\n";
	_stream << _indentation << _prefix << "symbolFlags = " << _symbol.symbolFlags << ";\n";
	return true;
}

coResult coCppTypesGeneratorPlugin::WriteParsedType(coStringOutputStream& _stream, const coParsedType& _parsedType, const coConstString& _indentation)
{
	const coType* type = _parsedType.type;
	coASSERT(type);

	coTRY(WriteSymbol(_stream, *type, "\t", "type->"), nullptr);
	_stream << _indentation << "type->size8 = sizeof(" << type->name << ");\n";
	_stream << _indentation << "type->alignment8 = alignof(" << type->name << ");\n";
	_stream << _indentation << "type->super = nullptr;\n";
	_stream << _indentation << "type->createFunc = []() -> void* { return new " << type->name << "(); };\n";

	_stream << _indentation << "\n";

	// Fields
	{
		coUint nbPublicFields = 0;
		for (const coParsedField* parsedField : _parsedType.parsedFields)
		{
			coASSERT(parsedField);
			const coField* field = parsedField->field;
			if (field->symbolFlags & coSymbol::public_)
			{
				++nbPublicFields;
			}
		}

		if (nbPublicFields)
		{
			_stream << _indentation << "// Fields\n";
			_stream << _indentation << "coReserve(type->fields, " << nbPublicFields << ");\n";
			_stream << _indentation << "\n";
			for (const coParsedField* parsedField : _parsedType.parsedFields)
			{
				coASSERT(parsedField);
				const coField* field = parsedField->field;
				coASSERT(field);
				if (field->symbolFlags & coSymbol::public_)
				{
					coTRY(WriteParsedField(_stream, _parsedType, *parsedField, _indentation), "Failed to write field: " << parsedField->field->name);
				}
			}
		}
	}

	return true;
}

coResult coCppTypesGeneratorPlugin::WriteParsedField(coStringOutputStream& _stream, const coParsedType& _parsedType, const coParsedField& _parsedField, const coConstString& _indentation)
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
	_stream << blockIndent << "field->offset8 = static_cast<decltype(field->offset8)>(coGetFieldOffset<" << _parsedType.type->name << ">(&" << _parsedType.type->name << "::" << field->name << "));\n";

	_stream << blockIndent << "\n";
	_stream << blockIndent << "coPushBack(type->fields, field);\n";
	_stream << _indentation << "}\n";
	_stream << _indentation << "\n";
	coASSERT(field);
	return true;
}

