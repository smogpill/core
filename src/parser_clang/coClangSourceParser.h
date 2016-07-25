// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "parser/source/coSourceParser.h"

class coSymbol;
class coParsedFunction;
class coParsedField;
class coParsedType;

class coClangSourceParser : public coSourceParser
{
	typedef coSourceParser Super;
public:
	coClangSourceParser();
	~coClangSourceParser();

	virtual coResult Parse(ParseResult& _result, const ParseConfig& _config) override;

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	struct ScopeInfo
	{
		ScopeInfo() : parser(nullptr), curType(nullptr), result(nullptr), cursor(nullptr) {}
		coClangSourceParser* parser;
		coParsedType* curType;
		ParseResult* result;
		const CXCursor* cursor;
	};
	coResult InitCommonParseArgs(const InitConfig& _config);
	coResult InitPrecompiledHeader(const InitConfig& _config);
	coResult InitSourceParseArgs(const InitConfig& _config);
	coResult ParseTypeChild(const ScopeInfo& scope, const CXCursor& _cursor);
	coResult ParseMethod(coParsedFunction& _parsedFunction, const CXCursor& _cursor);
	coResult ParseSymbol(coSymbol& _symbol, const CXCursor& _cursor);
	coResult ParseField(coParsedField& _parsedField, const CXCursor& _cursor);
	coResult ParseType(ParseResult& _result, const CXCursor& _cursor);
	coResult ParseTypes(ParseResult& _result, const CXCursor& _cursor);
	static CXCursor FindAttribute(const CXCursor& _cursor, const coConstString& _attr);

	static CXChildVisitResult ParseTypesVisitor(CXCursor _child, CXCursor _parent, CXClientData _clientData);
	static CXChildVisitResult ParseTypeChildrenVisitor(CXCursor _child, CXCursor _parent, CXClientData _clientData);

	CXIndex clangIndex;
	coDynamicString buildDir;
	coDynamicString precompiledHeaderPath;
	coDynamicArray<const coChar*> commonParseArgs;
	coDynamicArray<const coChar*> sourceParseArgs;
	coDynamicArray<coDynamicString*> stringBuffer;
};
