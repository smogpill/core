// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "parser_clang/pch.h"
#include "parser_clang/coClangReflectParser.h"
#include "pattern/scope/coDefer.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coType.h"
#include "lang/reflect/coField.h"
#include "parser/reflect/coParsedType.h"
#include "parser/reflect/coParsedField.h"
#include "container/string/coDynamicString_f.h"
#include "container/array/coDynamicArray_f.h"
#include "container/array/coConstArray_f.h"

coReflectParser* coReflectParser::Create()
{
	return new coClangReflectParser();
}

coClangReflectParser::coClangReflectParser()
	: clangIndex(nullptr)
{

}

coResult coClangReflectParser::OnInit(const coObject::InitConfig&)
{
	coTRY(!clangIndex, nullptr);
	clangIndex = clang_createIndex(true, false);
	return true;
}

coResult coClangReflectParser::Parse(const ParseConfig& _config)
{
	coTRY(Super::Parse(_config), nullptr);
	coDynamicString filePath(_config.filePath);
	coNullTerminate(filePath);
	CXTranslationUnit translationUnit = nullptr;
	const coConstArray<const coChar*> args = {"c++", "-std=c++11"};
	translationUnit = clang_createTranslationUnitFromSourceFile(clangIndex, filePath.data, args.count, args.data, 0, nullptr);
	coTRY(translationUnit, "Can't create translation unit from source file: " << filePath);
	coDEFER() { clang_disposeTranslationUnit(translationUnit); };
	CXCursor cursor = clang_getTranslationUnitCursor(translationUnit);
	coTRY(ParseTypes(cursor), "Failed to parse types");
	return true;
}

CXChildVisitResult coClangReflectParser::ParseTypesVisitor(CXCursor _child, CXCursor /*_parent*/, CXClientData _clientData)
{
	const ScopeInfo* scope = static_cast<const ScopeInfo*>(_clientData);
	coClangReflectParser* _this = scope->parser;
	const coBool definition = clang_isCursorDefinition(_child) != 0;
	if (!definition)
		return CXChildVisitResult::CXChildVisit_Continue;

	switch (_child.kind)
	{
	case CXCursor_ClassDecl:
	case CXCursor_StructDecl:
	{
		if (!_this->ParseType(_child))
		{
			coERROR("Failed");
			return CXChildVisitResult::CXChildVisit_Break;
		}
		break;
	}
	}
	return CXChildVisitResult::CXChildVisit_Continue;
}

CXChildVisitResult coClangReflectParser::ParseTypeChildrenVisitor(CXCursor _child, CXCursor /*_parent*/, CXClientData _clientData)
{
	const ScopeInfo* scope = static_cast<const ScopeInfo*>(_clientData);
	coASSERT(scope);
	coClangReflectParser* _this = scope->parser;
	if (!_this->ParseTypeChild(*scope, _child))
	{
		coERROR("Failed to parse type child");
		CXChildVisitResult::CXChildVisit_Break;
	}
	
	return CXChildVisitResult::CXChildVisit_Continue;
}

coResult coClangReflectParser::ParseTypeChild(const ScopeInfo& scope, const CXCursor& _cursor)
{
	coTRY(scope.curType, nullptr);
	switch (_cursor.kind)
	{
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

coResult coClangReflectParser::ParseTypes(const CXCursor& _cursor)
{
	ScopeInfo scope;
	scope.parser = this;
	clang_visitChildren(_cursor, &ParseTypesVisitor, &scope);
	return true;
}

coResult coClangReflectParser::ParseType(const CXCursor& _cursor)
{
	coParsedType* parsedType = new coParsedType();
	coDEFER() { delete parsedType; };
	parsedType->type = new coType();
	ScopeInfo scope;
	scope.parser = this;
	scope.curType = parsedType;
	clang_visitChildren(_cursor, &ParseTypeChildrenVisitor, &scope);
	coPushBack(parsedTypes, parsedType);
	return true;
}

coResult coClangReflectParser::ParseField(coParsedField& _parsedField, const CXCursor& _cursor)
{
	coTRY(ParseSymbol(*_parsedField.field, _cursor), nullptr);
	const CXType type = clang_getCursorType(_cursor);
	const CXCursor typeCursor = clang_getTypeDeclaration(type);
	const CXString typeSpelling = clang_getCursorSpelling(typeCursor);
	_parsedField.typeName = clang_getCString(typeSpelling);
	return true;
}

coResult coClangReflectParser::ParseSymbol(coSymbol& _symbol, const CXCursor& _cursor)
{
	const CXString name = clang_getCursorSpelling(_cursor);
	_symbol.name = clang_getCString(name);

	const CX_CXXAccessSpecifier access = clang_getCXXAccessSpecifier(_cursor);
	if (access == CX_CXXPublic)
		_symbol.symbolFlags |= coType::PUBLIC;
	return true;
}

coResult coClangReflectParser::ParseMethod(coParsedFunction& /*_function*/, const CXCursor& _cursor)
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
