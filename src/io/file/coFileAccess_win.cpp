// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/file/coFileAccess.h"
#include "io/path/coPathStatus.h"
#include "io/path/coPath_f.h"
#include "debug/log/coLog.h"
#include "platform/coOs.h"
#include "lang/result/coResult_f.h"
#include "container/string/coConstString16.h"
#include "container/string/coDynamicString16.h"
#include "container/string/coDynamicString16_f.h"

coResult coFileAccess::GetSize8(coInt64& _size8) const
{
	_size8 = 0;
	const HANDLE& handle = static_cast<const HANDLE&>(impl);
	coTRY(handle != INVALID_HANDLE_VALUE, nullptr);

	LARGE_INTEGER info;
	const BOOL res = ::GetFileSizeEx(handle, &info);
	if (res == FALSE)
	{
		coDynamicString str;
		coDumpLastOsError(str);
		coERROR("Failed to get the size of the file: " << str);
		return false;
	}

	_size8 = info.QuadPart;

	return true;
}

coResult coFileAccess::Write(const coByte* _data, coUint _size8)
{
	HANDLE& handle = static_cast<HANDLE&>(impl);
	coTRY(handle != INVALID_HANDLE_VALUE, nullptr);
	DWORD writtenSize8 = 0;
	const BOOL res = ::WriteFile(handle, _data, _size8, &writtenSize8, nullptr);
	if (res == FALSE)
	{
		coDynamicString str;
		coDumpLastOsError(str);
		coERROR("Failed to write to the file " << GetDebugName() << ": " << str);
		return false;
	}
	coTRY(_size8 == writtenSize8, nullptr);
	return true;
}

coResult coFileAccess::Read(coUint& _readSize8, coByte* _data, coUint _size8)
{
	HANDLE& handle = static_cast<HANDLE&>(impl);
	coTRY(handle != INVALID_HANDLE_VALUE, nullptr);
	_readSize8 = 0;
	DWORD readSize8 = 0;
	const BOOL res = ::ReadFile(handle, _data, _size8, &readSize8, nullptr);
	if (res == FALSE)
	{
		coDynamicString str;
		coDumpLastOsError(str);
		coERROR("Failed to read from the file " << GetDebugName() << ": " << str);
		return false;
	}
	_readSize8 = readSize8;
	return true;
}

coResult coFileAccess::GetTime(coUint64& _creationTime, coUint64& _lastAccessTime, coUint64& _lastWriteTime) const
{
	const HANDLE& handle = static_cast<const HANDLE&>(impl);
	coTRY(handle != INVALID_HANDLE_VALUE, nullptr);
	_creationTime = 0;
	_lastAccessTime = 0;
	_lastWriteTime = 0;
	FILETIME creationTime;
	FILETIME lastAccessTime;
	FILETIME lastWriteTime;
	const BOOL res = ::GetFileTime(handle, &creationTime, &lastAccessTime, &lastWriteTime);
	if (res == FALSE)
	{
		coDynamicString str;
		coDumpLastOsError(str);
		coERROR("Failed to get time from file: " << str);
		return false;
	}
	_creationTime = static_cast<coUint64>(creationTime.dwHighDateTime) << 32 | creationTime.dwLowDateTime;
	_lastAccessTime = static_cast<coUint64>(lastAccessTime.dwHighDateTime) << 32 | lastAccessTime.dwLowDateTime;
	_lastWriteTime = static_cast<coUint64>(lastWriteTime.dwHighDateTime) << 32 | lastWriteTime.dwLowDateTime;
	return true;
}

coResult coFileAccess::OnImplInit(const InitConfig& /*_config*/)
{
	HANDLE& handle = static_cast<HANDLE&>(impl);

	// Open mode
	DWORD openMode = 0;
	switch (mode)
	{
	case coFileAccess::Mode::read: openMode = GENERIC_READ; break;
	case coFileAccess::Mode::write: openMode = GENERIC_WRITE; break;
	default:
	{
		coERROR("File mode not supported: " << mode);
		return false;
	}
	}

	// Creation disposition
	DWORD creationDisposition = 0;
	{
		coPathStatus pathStatus;
		coGetPathStatus(pathStatus, path);
		switch (mode)
		{
		case coFileAccess::Mode::read:
		{
			if (pathStatus.Exists())
			{
				creationDisposition = OPEN_EXISTING;
			}
			else
			{
				coERROR("File does not exist.");
				return false;
			}
		}
		break;
		case coFileAccess::Mode::write:
		{
			if (pathStatus.Exists())
				creationDisposition = OPEN_EXISTING;
			else
				creationDisposition = CREATE_NEW;
		}
		break;
		default:
		{
			coERROR("File mode not supported: " << mode);
			return false;
		}
		}
	}

	// Flags and attributes
	const DWORD flagsAndAttributes = (mode == coFileAccess::Mode::read) ? FILE_ATTRIBUTE_READONLY : FILE_ATTRIBUTE_TEMPORARY;

	coDynamicString16 p;
	coSetFromUTF8(p, path);
	coNullTerminate(p);
	handle = ::CreateFileW(p.data, openMode, FILE_SHARE_READ, nullptr, creationDisposition, flagsAndAttributes, nullptr);
	if (handle == INVALID_HANDLE_VALUE)
	{
		coDynamicString str;
		coDumpLastOsError(str);
		coERROR("Failed to open the file: " << str);
		return false;
	}

	return true;
}

void coFileAccess::OnImplDestruct()
{
	HANDLE& handle = static_cast<HANDLE>(impl);
	if (handle != INVALID_HANDLE_VALUE)
	{
		const BOOL res = ::CloseHandle(handle);
		if (res == FALSE)
		{
			coDynamicString str;
			coDumpLastOsError(str);
			coERROR("Failed to close the mapped file: " << str);
		}
		handle = INVALID_HANDLE_VALUE;
	}
}

void coFileAccess::OnImplConstruct()
{
	impl = INVALID_HANDLE_VALUE;
}
