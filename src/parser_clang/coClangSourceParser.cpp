// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "parser_clang/pch.h"
#include "parser_clang/coClangSourceParser.h"
#include "pattern/scope/coDefer.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coType.h"
#include "lang/reflect/coField.h"
#include "parser/source/coParsedType.h"
#include "parser/source/coParsedField.h"
#include "container/string/coDynamicString_f.h"
#include "container/array/coDynamicArray_f.h"
#include "container/array/coConstArray_f.h"
#include "memory/allocator/coLocalAllocator.h"
#include "io/path/coPath_f.h"
#include "io/path/coPathStatus.h"
#include "io/dir/coDirectory_f.h"

static coConstString co_GetClangErrorString(CXErrorCode _errorCode)
{
	switch (_errorCode)
	{
	case CXError_Success: return "";
	case CXError_Failure: return "Generic error, no further details are available.";
	case CXError_Crashed: return "Libclang crashed while performing the requested operation.";
	case CXError_InvalidArguments: return "The function detected that the arguments violate the function contract.";
	case CXError_ASTReadError: return "An AST deserialization error has occurred.";
	default: return "Unknown error code";
	}
}

coSourceParser* coSourceParser::Create()
{
	return new coClangSourceParser();
}

coClangSourceParser::coClangSourceParser()
	: clangIndex(nullptr)
{

}

coClangSourceParser::~coClangSourceParser()
{
	if (clangIndex)
	{
		clang_disposeIndex(clangIndex);
	}
	coDeleteElementsAndClear(stringBuffer);
}

coResult coClangSourceParser::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);

	coASSERT(!clangIndex);
	clangIndex = clang_createIndex(1, 1);

	buildDir = config.buildDir;
	coTRY(coCreateDirsIfMissing(buildDir), "Failed to create the build directory: "<<buildDir);

	coTRY(InitCommonParseArgs(config), nullptr);
	if (config.precompiledHeaderSourcePath != "")
	{
		coTRY(InitPrecompiledHeader(config), nullptr);
	}
	coTRY(InitSourceParseArgs(config), nullptr);

	return true;
}

coResult coClangSourceParser::InitCommonParseArgs(const InitConfig& _config)
{
	coTRY(commonParseArgs.count == 0, nullptr);
	commonParseArgs = { "-x", "c++", "-std=c++11", "-fms-compatibility-version=19", "-D", "coREFLECTION_PARSING" };
	for (const coConstString& includeDir : _config.includeDirs)
	{
		coDynamicString* s = new coDynamicString();
		coPushBack(stringBuffer, s);
		coTRY(includeDir != "", "Null include dir specified.");
		*s = "-I";
		*s << includeDir;
		coNullTerminate(*s);
		coPushBack(commonParseArgs, static_cast<const coChar*>(s->data));
	}

	return true;
}

coResult coClangSourceParser::InitSourceParseArgs(const InitConfig& /*_config*/)
{
	coTRY(sourceParseArgs.count == 0, nullptr);
	coPushBackArray(sourceParseArgs, commonParseArgs);
	if (precompiledHeaderPath != "")
	{
		coPushBack(sourceParseArgs, "-include-pch");
		coNullTerminate(precompiledHeaderPath);
		coPushBack(sourceParseArgs, precompiledHeaderPath.data);
	}
	return true;
}

coResult coClangSourceParser::InitPrecompiledHeader(const InitConfig& _config)
{
	coLocalAllocator localAllocator(2048);

	// Check out dir
	/*{
	coPathStatus status;
	coTRY(coGetPathStatus(status, _config.outPath), nullptr);
	coTRY(status.Exists(), "The output directory does not exist: "<<_config.outPath);
	}*/

	// Build file path
	coDynamicString filePath(localAllocator);
	{
		filePath = _config.precompiledHeaderSourcePath;
		coTRY(coIsFile(filePath), "Not a file: " << filePath);
		coNullTerminate(filePath);
	}

	// Build precompiled header path
	{
		coConstString baseName;
		coGetBaseName(baseName, _config.precompiledHeaderSourcePath);
		coJoinPaths(precompiledHeaderPath, buildDir, baseName);
		precompiledHeaderPath << ".pch";
		coNullTerminate(precompiledHeaderPath);
	}

	// Parse
	{
		CXTranslationUnit translationUnit = nullptr;
		//translationUnit = clang_createTranslationUnitFromSourceFile(clangIndex, filePath.data, commonParseArgs.count, commonParseArgs.data, 0, nullptr);
		const CXErrorCode parseError = clang_parseTranslationUnit2(clangIndex, filePath.data, commonParseArgs.data, commonParseArgs.count, nullptr, 0, CXTranslationUnit_SkipFunctionBodies | CXTranslationUnit_Incomplete, &translationUnit);
		coTRY(parseError == CXError_Success, "Clang failed to parse the file: " << filePath << " (libclang: "<< co_GetClangErrorString(parseError) << ")");
		coTRY(translationUnit, "Can't create translation unit from source file: " << filePath);
		coDEFER() { clang_disposeTranslationUnit(translationUnit); };
		const CXSaveError saveError = static_cast<CXSaveError>(clang_saveTranslationUnit(translationUnit, precompiledHeaderPath.data, 0));
		coTRY(saveError == CXSaveError_None, "Clang failed to save the file: " << precompiledHeaderPath.data);
	}

	return true;
}

coResult coClangSourceParser::Parse(ParseResult& _result, const ParseConfig& _config)
{
	coTRY(Super::Parse(_result, _config), nullptr);
	coLocalAllocator localAllocator(2048);

	coDynamicString filePath(localAllocator);
	filePath = _config.filePath;
	coNullTerminate(filePath);
	CXTranslationUnit translationUnit = nullptr;
	//translationUnit = clang_createTranslationUnitFromSourceFile(clangIndex, filePath.data, sourceParseArgs.count, sourceParseArgs.data, 0, nullptr);
	const CXErrorCode error = clang_parseTranslationUnit2(clangIndex, filePath.data, sourceParseArgs.data, sourceParseArgs.count, nullptr, 0, CXTranslationUnit_SkipFunctionBodies | CXTranslationUnit_Incomplete, &translationUnit);
	coTRY(error == CXError_Success, "Clang failed to parse the file: " << filePath << " (libclang: " << co_GetClangErrorString(error) << ")");
	coTRY(translationUnit, "Can't create translation unit from source file: " << filePath);
	coDEFER() { clang_disposeTranslationUnit(translationUnit); };
	CXCursor cursor = clang_getTranslationUnitCursor(translationUnit);

	if (_result.parsedTypes)
	{
		coTRY(ParseTypes(_result, cursor), "Failed to parse types");
	}
	return true;
}

CXChildVisitResult coClangSourceParser::ParseTypesVisitor(CXCursor _child, CXCursor /*_parent*/, CXClientData _clientData)
{
	const ScopeInfo* scope = static_cast<const ScopeInfo*>(_clientData);
	coClangSourceParser* _this = scope->parser;
	const coBool definition = clang_isCursorDefinition(_child) != 0;
	if (!definition)
		return CXChildVisitResult::CXChildVisit_Continue;

	switch (_child.kind)
	{
	case CXCursor_ClassDecl:
	case CXCursor_StructDecl:
	{
		const CXCursor reflectedAttr = FindAttribute(_child, "Reflected");
		if (!clang_Cursor_isNull(reflectedAttr))
		{
			coASSERT(scope->result);
			if (!_this->ParseType(*scope->result, _child))
			{
				coERROR("Failed");
				return CXChildVisitResult::CXChildVisit_Break;
			}
		}
		break;
	}
	}
	return CXChildVisitResult::CXChildVisit_Continue;
}

CXChildVisitResult coClangSourceParser::ParseTypeChildrenVisitor(CXCursor _child, CXCursor /*_parent*/, CXClientData _clientData)
{
	const ScopeInfo* scope = static_cast<const ScopeInfo*>(_clientData);
	coASSERT(scope);
	coClangSourceParser* _this = scope->parser;
	if (!_this->ParseTypeChild(*scope, _child))
	{
		coERROR("Failed to parse type child");
		CXChildVisitResult::CXChildVisit_Break;
	}
	
	return CXChildVisitResult::CXChildVisit_Continue;
}

coResult coClangSourceParser::ParseTypeChild(const ScopeInfo& scope, const CXCursor& _cursor)
{
	coTRY(scope.curType, nullptr);
	switch (_cursor.kind)
	{
	case CXCursor_StructDecl:
	{
		int x = 0;
		++x;
		break;
	}
	case CXCursor_FieldDecl:
	{
		coParsedField* parsedField = new coParsedField();
		parsedField->field = new coField();
		coDEFER() { delete parsedField; };
		coTRY(ParseField(*parsedField, _cursor), "Failed to parse field: " << parsedField->field->name);
		coPushBack(scope.curType->parsedFields, parsedField);
		parsedField = nullptr;
		break;
	}
	case CXCursor_VarDecl: // Static field
	{
		break;
	}
	case CXCursor_CXXMethod: // Both static and non-static
	{
		const coBool staticMethod = clang_CXXMethod_isStatic(_cursor) != 0;
		if (staticMethod)
		{

		}
		else
		{

		}
		break;
	}
	}
	return true;
}

coResult coClangSourceParser::ParseTypes(ParseResult& _result, const CXCursor& _cursor)
{
	coASSERT(_result.parsedTypes);
	ScopeInfo scope;
	scope.parser = this;
	scope.result = &_result;
	clang_visitChildren(_cursor, &ParseTypesVisitor, &scope);
	return true;
}

coResult coClangSourceParser::ParseType(ParseResult& _result, const CXCursor& _cursor)
{
	coParsedType* parsedType = new coParsedType();
	coDEFER() { delete parsedType; };
	parsedType->type = new coType();
	coTRY(ParseSymbol(*parsedType->type, _cursor), nullptr);
	ScopeInfo scope;
	scope.parser = this;
	scope.curType = parsedType;
	clang_visitChildren(_cursor, &ParseTypeChildrenVisitor, &scope);
	coPushBack(*_result.parsedTypes, parsedType);
	parsedType = nullptr; // Release from defer
	return true;
}

coResult coClangSourceParser::ParseField(coParsedField& _parsedField, const CXCursor& _cursor)
{
	coTRY(ParseSymbol(*_parsedField.field, _cursor), nullptr);
	const CXType type = clang_getCursorType(_cursor);
	const CXType canonicalType = clang_getCanonicalType(type);
	const CXCursor typeCursor = clang_getTypeDeclaration(canonicalType);
	const CXString typeSpelling = clang_getCursorSpelling(typeCursor);
	coDEFER() { clang_disposeString(typeSpelling); };
	_parsedField.typeName = clang_getCString(typeSpelling);
	return true;
}

coResult coClangSourceParser::ParseSymbol(coSymbol& _symbol, const CXCursor& _cursor)
{
	const CXString name = clang_getCursorSpelling(_cursor);
	coDEFER() { clang_disposeString(name); };
	_symbol.name = clang_getCString(name);

	const CX_CXXAccessSpecifier access = clang_getCXXAccessSpecifier(_cursor);
	if (access == CX_CXXPublic)
		_symbol.symbolFlags |= coType::public_;
	return true;
}

coResult coClangSourceParser::ParseMethod(coParsedFunction& /*_function*/, const CXCursor& _cursor)
{
	const coBool staticMethod = clang_CXXMethod_isStatic(_cursor) != 0;
	if (staticMethod)
	{

	}
	else
	{

	}
	return true;
}

CXCursor coClangSourceParser::FindAttribute(const CXCursor& _cursor, const coConstString& _attr)
{
	switch (_cursor.kind)
	{
	case CXCursor_ClassDecl:
	case CXCursor_StructDecl:
	{
		struct Result
		{
			Result(const coConstString& _structName) : cursor(clang_getNullCursor()), structName(_structName) {}
			CXCursor cursor;
			const coConstString& structName;
		};
		static const coConstString attributePrefix("_attribute_");
		coDynamicString structName;
		structName << attributePrefix;
		structName << _attr;
		Result result(structName);
		auto AttributeVisitor = [](CXCursor _child, CXCursor /*_parent*/, CXClientData _clientData)
		{
			if (_child.kind == CXCursor_StructDecl)
			{
				CXString cursorStr = clang_getCursorDisplayName(_child);
				coDEFER() { clang_disposeString(cursorStr); };
				const coConstString name = clang_getCString(cursorStr);
				Result* result = static_cast<Result*>(_clientData);
				if (name == result->structName)
				{
					result->cursor = _child;
					return CXChildVisit_Break;
				}
			}
			return CXChildVisit_Continue;
		};
		clang_visitChildren(_cursor, AttributeVisitor, &result);
		return result.cursor;
		break;
	}
	}

	return clang_getNullCursor();
}
