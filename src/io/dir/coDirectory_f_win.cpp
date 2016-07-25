// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/dir/coDirectory_f.h"
#include "container/string/coDynamicString16.h"
#include "container/string/coDynamicString16_f.h"
#include "platform/coOs.h"

coResult coCreateDir(const coConstString& _path)
{
	coDynamicString16 path;
	coSetFromUTF8(path, _path);
	coNullTerminate(path);
	const BOOL ret = ::CreateDirectoryW(path.data, NULL);
	if (ret)
	{
		coDynamicString s;
		coDumpLastOsError(s);
		coERROR("Failed to create the directory: " << _path << " (" << s << ")");
		return false;
	}
	return true;
}
