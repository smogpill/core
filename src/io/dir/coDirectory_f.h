// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/result/coResult.h"

class coConstString;

enum class coDefaultDir
{
	CURRENT,
	LOCAL_APP_DATA,
	ROAMING_APP_DATA,
	SAVEGAME,
	TEMP,

	END
};
extern coDynamicString _co_defaultDirs[];

coResult coInitDefaultDirs();
coResult coCreateDir(const coConstString& _path);
coResult coCreateDirsIfMissing(const coConstString& _rawPath);
coResult coDeleteDir(const coConstString& path);
const coConstString& coGetDefaultDir(coDefaultDir dir);
