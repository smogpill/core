// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "coFile_f.h"
#include "../path/coPath_f.h"
#include "container/string/coDynamicString16_f.h"
#include "platform/coOs.h"

coBool coIsWritable(const coConstString& path)
{
	// Does not check recursively. The file can be set to be writable, but if the parent folder is not, then it won't be.
	coDynamicString16 p;
	coSetFromUTF8(p, path);
	coNullTerminate(p);
	const DWORD attributes = ::GetFileAttributesW(p.data);
	return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_READONLY) == 0;
}

coResult coDeleteFile(const coConstString& path)
{
	coDynamicString16 p;
	coSetFromUTF8(p, path);
	coNullTerminate(p);

	const BOOL res = coIsDirectory(path) ? RemoveDirectoryW(p.data) : DeleteFileW(p.data);
	if (!res)
	{
		coERROR("Failed to delete '" << path << "': " << coGetLastOSErrorMessage());
		return false;
	}
	return true;
// 	coDynamicString path2(path);
// 	coNullTerminate(path2);
// 	coTRY(std::remove(path2.data) == 0, "Failed to delete " << path);
// 	return true;
}
