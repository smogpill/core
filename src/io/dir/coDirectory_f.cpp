// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "coDirectory_f.h"
#include "coDirectoryAccess.h"
#include "../path/coPath_f.h"
#include "../file/coFile_f.h"
#include "lang/result/coResult.h"
#include "lang/result/coResult_f.h"

extern _coDefaultDirs* _co_defaultDirs = nullptr;

coResult coCreateDirsIfMissing(const coConstString& _rawPath)
{
	coASSERT(coIsPathNormalized(_rawPath));

	coConstString path = _rawPath;

	{
		coConstString ext;
		coGetExtension(ext, path);
		if (ext != "")
		{
			coGetParentDir(path, path);
		}
	}
	
	if (!coExists(path))
	{
		coConstString parentDir;
		coGetParentDir(parentDir,path);
		coTRY(coCreateDirsIfMissing(parentDir), "Failed to create: "<<parentDir);
		coTRY(coCreateDir(path), "Failed to create: "<<path);
	}
	return true;
}

coResult coDeleteDir(const coConstString& path)
{
	coDirectoryAccess dir;
	coDirectoryAccess::InitConfig c;
	c.path = path;
	coTRY(dir.Init(c), nullptr);
	coDynamicString s;
	for (const auto& entry : dir)
	{
		coJoinPaths(s, path, entry.name);
		if (coIsDirectory(s))
		{
			coTRY(coDeleteDir(s), nullptr);
		}
		else
		{
			coTRY(coDeleteFile(s), nullptr);
		}
	}
	return coDeleteFile(path);
}

const coConstString& coGetDefaultDir(coDefaultDir dir)
{
	coASSERT(_co_defaultDirs);
	return _co_defaultDirs->dirs[coUint(dir)];
}
