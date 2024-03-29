// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/dir/coDirectory_f.h"
#include "io/path/coPath_f.h"
#include "container/string/coDynamicString16.h"
#include "container/string/coDynamicString16_f.h"
#include "container/string/coConstString16.h"
#include "pattern/pointer/coUniquePtr.h"
#include "pattern/scope/coScopeExit.h"
#include "platform/coOs.h"
#include "debug/log/coLog.h"
#include "lang/result/coResult_f.h"

coResult coCreateDir(const coConstString& _path)
{
	coDynamicString16 path;
	coSetFromUTF8(path, _path);
	coNullTerminate(path);
// 	SECURITY_ATTRIBUTES securityAttributes{};
// 	securityAttributes.bInheritHandle = false;
// 	securityAttributes.
	if (::CreateDirectoryW(path.data, nullptr) == 0)
	{
		coERROR("Failed to create the directory: " << _path << ": " << coGetLastOSErrorMessage());
		return false;
	}
	return true;
}

coResult _coInitKnownDir(coDefaultDir defaultDir)
{
	KNOWNFOLDERID id{};
	switch (defaultDir)
	{
	case coDefaultDir::EXECUTABLE:
	{
		coClearLastOSError();
		coWideChar path[MAX_PATH];
		GetModuleFileName(nullptr, path, MAX_PATH);
		if (coLastOSErrorExists())
		{
			coERROR("Failed to get the executable path using GetModuleFileName(): " << coGetLastOSErrorMessage());
			return false;
		}
		coDynamicString filePath;
		coSetFromWide(filePath, coConstString16(path));
		coNormalizePath(filePath);
		coDynamicString& dir = _co_defaultDirs->dirs[coUint(coDefaultDir::EXECUTABLE)];
		dir = coGetParentDir(filePath);
		coNormalizePath(dir);
		return true;
	}
	case coDefaultDir::LOCAL_APP_DATA: id = FOLDERID_LocalAppData; break;
	case coDefaultDir::ROAMING_APP_DATA: id = FOLDERID_RoamingAppData; break;
	case coDefaultDir::SAVEGAME: id = FOLDERID_SavedGames; break;
	default:
	{
		coERROR("Unknown default dir: " << coInt(defaultDir));
		return false;
	}
	}

	coWideChar* path = nullptr;
	coSCOPE_EXIT(CoTaskMemFree(path));
	coTRY(SHGetKnownFolderPath(id, KF_FLAG_CREATE, nullptr, &path) == S_OK, "SHGetKnownFolderPath() failed.");
	coDynamicString& dir = _co_defaultDirs->dirs[coUint(defaultDir)];
	coSetFromWide(dir, coConstString16(path));
	coNormalizePath(dir);
	return true;
}

coResult _coInitCurrentDir()
{
	DWORD size = GetCurrentDirectoryW(0, nullptr);
	if (size == 0)
	{
		coERROR("Failed to get the current directory path size: " << coGetLastOSErrorMessage());
		return false;
	}
	coUniquePtr<coWideChar> path(new coWideChar[size]);
	coTRY(GetCurrentDirectory(size, path.Get()), nullptr);

	coDynamicString& dir = _co_defaultDirs->dirs[coUint(coDefaultDir::CURRENT)];
	coSetFromWide(dir, coConstString16(path.Get(), size));
	coNormalizePath(dir);
	return true;
}

coResult _coInitTempDir()
{
	coWideChar path[MAX_PATH + 1];
	DWORD size = GetTempPathW(coARRAY_SIZE(path), path);
	if (size == 0)
	{
		coERROR("Failed to get the temp directory path size: " << coGetLastOSErrorMessage());
		return false;
	}
	coDynamicString& dir = _co_defaultDirs->dirs[coUint(coDefaultDir::TEMP)];
	coSetFromWide(dir, coConstString16(path, size));
	coNormalizePath(dir);
	return true;
}

coResult coInitDefaultDirs()
{
	if (_co_defaultDirs)
		return true;
	_co_defaultDirs = new _coDefaultDirs();
	coTRY(_coInitCurrentDir(), "Failed to init the current directory.");
	coTRY(_coInitTempDir(), "Failed to init the temp directory.");
	coTRY(_coInitKnownDir(coDefaultDir::EXECUTABLE), "Failed to init the executable directory.");
	coTRY(_coInitKnownDir(coDefaultDir::LOCAL_APP_DATA), "Failed to init the local app data directory.");
	coTRY(_coInitKnownDir(coDefaultDir::ROAMING_APP_DATA), "Failed to init the app data directory.");
	coTRY(_coInitKnownDir(coDefaultDir::SAVEGAME), "Failed to init the saved game directory.");
	return true;
}
