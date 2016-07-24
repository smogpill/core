// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "parser/reflect/coReflectParser.h"

class coSymbol;
class coParsedFunction;
class coParsedField;
class coParsedType;

class coClangReflectParser : public coReflectParser
{
	typedef coReflectParser Super;
public:
	coClangReflectParser();
	~coClangReflectParser();

	virtual coResult ParsePrecompiledHeader(const ParseConfig& _config) override;
	virtual coResult Parse(const ParseConfig& _config) override;

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	struct ScopeInfo
	{
		ScopeInfo() : parser(nullptr), curType(nullptr), cursor(nullptr) {}
		coClangReflectParser* parser;
		coParsedType* curType;
		const CXCursor* cursor;
	};
	coResult ParseTypeChild(const ScopeInfo& scope, const CXCursor& _cursor);
	coResult ParseMethod(coParsedFunction& _parsedFunction, const CXCursor& _cursor);
	coResult ParseSymbol(coSymbol& _symbol, const CXCursor& _cursor);
	coResult ParseField(coParsedField& _parsedField, const CXCursor& _cursor);
	coResult ParseType(const CXCursor& _cursor);
	coResult ParseTypes(const CXCursor& _cursor);
	static CXCursor FindAttribute(const CXCursor& _cursor, const coConstString& _attr);

	static CXChildVisitResult ParseTypesVisitor(CXCursor _child, CXCursor _parent, CXClientData _clientData);
	static CXChildVisitResult ParseTypeChildrenVisitor(CXCursor _child, CXCursor _parent, CXClientData _clientData);

	CXIndex clangIndex;
};
