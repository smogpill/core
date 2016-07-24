// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"
#include "container/string/coConstString.h"

class coReflectParser;

class coProjectParser : public coObject
{
	coDECLARE_DERIVED_TYPE(coProjectParser, coObject);
public:
	class InitConfig : public Super::InitConfig
	{
	public:
	};
	struct ParseConfig
	{
		ParseConfig();

		coConstString projectDir;
		coConstString outDir;
		coConstString precompiledHeaderRelativePath;
	};

	coProjectParser();
	virtual ~coProjectParser();
	coResult Parse(const ParseConfig& _config);

protected:
	virtual coResult OnInit(const coObject::InitConfig& _config) override;
private:
	coResult ParsePrecompileHeader(const ParseConfig& _config);
	coResult ParseSourceDir(const coConstString& _path);
	coResult ParseSourceFile(const coConstString& _path);

	coReflectParser* sourceParser;
};
