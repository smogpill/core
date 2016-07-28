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
#include "parser/source/coParsedType.h"

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
	coTRY(coIsPathNormalized(_config.srcReferenceDir), nullptr);
	coJoinPaths(srcProjectDir, _config.srcReferenceDir, _config.projectRelativePath);
	coTRY(coIsDirectory(srcProjectDir), "Failed to find the project source dir: " << srcProjectDir);

	coTRY(coIsPathNormalized(_config.outReferenceDir), nullptr);
	coJoinPaths(outProjectDir, _config.outReferenceDir, _config.projectRelativePath);
	coTRY(coCreateDirsIfMissing(outProjectDir), "Failed to create output directory: " << outProjectDir);

	// Build the PCH path
	coDynamicString pchPath;
	if (_config.precompiledHeaderPath != "")
	{
		coJoinPaths(pchPath, _config.srcReferenceDir, _config.precompiledHeaderPath);
		coTRY(coIsFile(pchPath), "Failed to find: " << pchPath);
	}

	// Init the parser
	coTRY(!sourceParser, nullptr);
	sourceParser = coSourceParser::Create();
	coDEFER() { delete sourceParser; sourceParser = nullptr; };
	{
		coSourceParser::InitConfig config;
		config.buildDir = outProjectDir;
		config.precompiledHeaderSourcePath = pchPath;
		config.includeDirs = { _config.srcReferenceDir };
		coTRY(sourceParser->Init(config), nullptr);
	}
	
	// Parse sources
	coTRY(ParseSourceDir(_out, _config, _config.projectRelativePath), nullptr);

	return true;
}

coResult coProjectParser::ParseSourceDir(coParsedProject& _out, const ParseConfig& _config, const coConstString& _relativePath)
{
	coDynamicString absolutePath;
	coJoinPaths(absolutePath, _config.srcReferenceDir, _relativePath);

	coDirectoryAccess dirAccess;
	{
		coDirectoryAccess::InitConfig c;
		c.path = absolutePath;
		coTRY(dirAccess.Init(c), "Failed to access:" << absolutePath);
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
			coJoinPaths(p, _relativePath, e.name);
			coTRY(ParseSourceDir(_out, _config, p), "Failed to parse the directory: " << p);
			break;
		}
		case coPathStatus::Status::regularFile:
		{
			coConstString ext;
			coGetExtension(ext, e.name);
			if (ext == ".h")
			{
				coDynamicString p;
				coJoinPaths(p, _relativePath, e.name);
				coTRY(ParseSourceFile(_out, _config, p), "Failed to parse the source file: " << p);
			}
			break;
		}
		}
	}
	
	return true;
}

coResult coProjectParser::ParseSourceFile(coParsedProject& _out, const ParseConfig& _config, const coConstString& _relativePath)
{
	coDynamicString absolutePath;
	coJoinPaths(absolutePath, _config.srcReferenceDir, _relativePath);

	coSourceParser::ParseConfig config;
	config.filePath = absolutePath;
	coSourceParser::ParseResult result;
	coDynamicArray<coParsedType*> parsedTypes;
	result.parsedTypes = &parsedTypes;
	coTRY(sourceParser->Parse(result, config), nullptr);
	for (coParsedType* parsedType : parsedTypes)
	{
		parsedType->sourcePath = _relativePath;
	}
	coPushBackArray(_out.parsedTypes, parsedTypes);
	return true;
}
