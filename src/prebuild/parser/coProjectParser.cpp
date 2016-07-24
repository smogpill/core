// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "prebuild/pch.h"
#include "prebuild/parser/coProjectParser.h"
#include "lang/result/coResult_f.h"
#include "io/dir/coDirectoryAccess.h"
#include "io/path/coPath_f.h"
#include "parser/reflect/coReflectParser.h"

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

	coTRY(!sourceParser, nullptr);
	sourceParser = coReflectParser::Create();
	{
		coReflectParser::InitConfig config;
		coTRY(sourceParser->Init(config), nullptr);
	}

	return true;
}

coResult coProjectParser::Parse(const ParseConfig& _config)
{
	coTRY(ParsePrecompileHeader(_config), nullptr);
	coTRY(ParseSourceDir(_config.projectDir), nullptr);
	return true;
}

coResult coProjectParser::ParsePrecompileHeader(const ParseConfig& _config)
{
	if (_config.precompiledHeaderRelativePath.count == 0)
		return true;

	coDynamicString pchPath;
	coJoinPaths(pchPath, _config.projectDir, _config.precompiledHeaderRelativePath);
	coPathStatus pathStatus;
	coTRY(coGetPathStatus(pathStatus, pchPath), "Failed to get the path status of: " << pchPath);
	if (pathStatus.Exists())
	{
		coReflectParser::ParseConfig config;
		config.filePath = pchPath;
		config.outPath = _config.outDir;
		coTRY(sourceParser->ParsePrecompiledHeader(config), "Failed to parse the precompiled header: " << pchPath);
	}
	return true;
}

coResult coProjectParser::ParseSourceDir(const coConstString& _path)
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
			coTRY(ParseSourceDir(p), "Failed to parse the directory: " << p);
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
				coTRY(ParseSourceFile(p), "Failed to parse the source file: " << p);
			}
			break;
		}
		}
	}
	
	return true;
}

coResult coProjectParser::ParseSourceFile(const coConstString& _path)
{
	coReflectParser::ParseConfig config;
	config.filePath = _path;
	coTRY(sourceParser->Parse(config), nullptr);
	return true;
}
