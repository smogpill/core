// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "parser/pch.h"
#include "parser/project/coProjectParser.h"
#include "parser/project/coParsedProject.h"
#include "lang/result/coResult_f.h"
#include "pattern/scope/coDefer.h"
#include "io/dir/coDirectoryAccess.h"
#include "io/dir/coDirectory_f.h"
#include "io/path/coPath_f.h"
#include "parser/source/coSourceParser.h"

coProjectParser::coProjectParser()
	: sourceParser(nullptr)
{

}

coProjectParser::~coProjectParser()
{
	delete sourceParser; sourceParser = nullptr;
}

coProjectParser::ParseConfig::ParseConfig()
{

}

coResult coProjectParser::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	return true;
}

coResult coProjectParser::Parse(coParsedProject& _out, const ParseConfig& _config)
{
	coTRY(coCreateDirsIfMissing(_config.outDir), "Failed to create output directory: " << _config.outDir);

	// Build the PCH path
	coDynamicString pchPath;
	if (_config.precompiledHeaderRelativePath != "")
	{
		coJoinPaths(pchPath, _config.projectDir, _config.precompiledHeaderRelativePath);
		coTRY(coIsFile(pchPath), "Failed to find: " << pchPath);
	}

	// Init the parser
	coTRY(!sourceParser, nullptr);
	sourceParser = coSourceParser::Create();
	coDEFER() { delete sourceParser; sourceParser = nullptr; };
	{
		coSourceParser::InitConfig config;
		config.buildDir = _config.outDir;
		config.precompiledHeaderSourcePath = pchPath;
		coHACK("Hardcoded paths");
		config.includeDirs = { "J:/CODE/core/src" };
		coTRY(sourceParser->Init(config), nullptr);
	}
	
	// Parse sources
	coTRY(ParseSourceDir(_out, _config.projectDir), nullptr);

	return true;
}

coResult coProjectParser::ParseSourceDir(coParsedProject& _out, const coConstString& _path)
{
	coDirectoryAccess dirAccess;
	{
		coDirectoryAccess::InitConfig c;
		c.path = _path;
		coTRY(dirAccess.Init(c), "Failed to access:" << _path);
	}

	for (const auto& e : dirAccess)
	{
		switch (e.status.status)
		{
		default:
		{
			break;
		}
		case coPathStatus::Status::directory:
		{
			coDynamicString p;
			coJoinPaths(p, _path, e.name);
			coTRY(ParseSourceDir(_out, p), "Failed to parse the directory: " << p);
			break;
		}
		case coPathStatus::Status::regularFile:
		{
			coConstString ext;
			coGetExtension(ext, e.name);
			if (ext == ".h")
			{
				coDynamicString p;
				coJoinPaths(p, _path, e.name);
				coTRY(ParseSourceFile(_out, p), "Failed to parse the source file: " << p);
			}
			break;
		}
		}
	}
	
	return true;
}

coResult coProjectParser::ParseSourceFile(coParsedProject& _out, const coConstString& _path)
{
	coSourceParser::ParseConfig config;
	config.filePath = _path;
	coSourceParser::ParseResult result;
	result.parsedTypes = &_out.parsedTypes;
	coTRY(sourceParser->Parse(result, config), nullptr);
	return true;
}
