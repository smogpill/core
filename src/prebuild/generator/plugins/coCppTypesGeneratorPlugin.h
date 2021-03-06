// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "prebuild/generator/coProjectGeneratorPlugin.h"

class coParsedProject;
class coParsedType;
class coParsedField;
class coDynamicString;
class coStringOutputStream;
class coSymbol;

class coCppTypesGeneratorPlugin : public coProjectGeneratorPlugin
{
	coDECLARE_SUPER(coProjectGeneratorPlugin);
public:
	virtual coResult Generate(const coParsedProject& _parsedProject) override;

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

private:
	coResult GenerateTypes(const coParsedProject& _parsedProject);
	coResult GenerateType(coDynamicString& _outPath, const coParsedType& _parsedType);
	coResult WriteTypeBuilderDeclaration(coStringOutputStream& _stream, const coParsedType& _parsedType);
	coResult WriteGetStaticTypeFunc(coStringOutputStream& _stream, const coParsedType& _parsedType);
	coResult WriteSymbol(coStringOutputStream& _stream, const coSymbol& _symbol, const coConstString& _indentation, const coConstString& _prefix);
	coResult WriteInitTypeFunc(coStringOutputStream& _stream, const coParsedType& _parsedType);
	coResult WriteLinkTypeFunc(coStringOutputStream& _stream, const coParsedType& _parsedType);
	coResult WriteParsedType(coStringOutputStream& _stream, const coParsedType& _parsedType, const coConstString& _indentation);
	coResult WriteParsedField(coStringOutputStream& _stream, const coParsedType& _parsedType, const coParsedField& _parsedField, const coConstString& _indentation);

	coDynamicString projectGenDir;
	coDynamicString typesGenDir;
};
