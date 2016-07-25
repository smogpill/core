// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/dir/coDirectory_f.h"
#include "io/path/coPath_f.h"
#include "lang/result/coResult.h"
#include "lang/result/coResult_f.h"

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
