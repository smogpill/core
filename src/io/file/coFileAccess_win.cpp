// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/file/coFileAccess.h"
#include "io/path/coPathStatus.h"
#include "io/path/coPath_f.h"
#include "debug/log/coLog.h"
#include "platform/coOs.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coNumericLimits.h"
#include "container/string/coConstString16.h"
#include "container/string/coDynamicString16.h"
#include "container/string/coDynamicString16_f.h"

coResult coFileAccess::GetSize8(coUint64& _size8) const
{
	_size8 = 0;
	const HANDLE& handle = static_cast<const HANDLE&>(impl);
	coTRY(handle != INVALID_HANDLE_VALUE, nullptr);

	LARGE_INTEGER info;
	const BOOL res = ::GetFileSizeEx(handle, &info);
	if (res == FALSE)
	{
		coERROR("Failed to get the size of the file: " << coGetLastOSErrorMessage());
		return false;
	}

	coNumericConvert(_size8, info.QuadPart);

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
		coERROR("Failed to get time from file: " << coGetLastOSErrorMessage());
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
		coERROR("File mode not supported: " << coUint32(mode));
		return false;
	}
	}

	// Creation disposition
	DWORD creationDisposition = 0;
	{
		switch (mode)
		{
		case coFileAccess::Mode::read:
		{
			if (coExists(path))
			{
				creationDisposition = OPEN_EXISTING;
			}
			else
			{
				coERROR("File does not exist: "<<path);
				return false;
			}
		}
		break;
		case coFileAccess::Mode::write:
		{
			if (coExists(path))
				creationDisposition = TRUNCATE_EXISTING;
			else
				creationDisposition = CREATE_NEW;
		}
		break;
		default:
		{
			coERROR("File mode "<<mode<<" not supported: " << path);
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
		coERROR("Failed to open the file: " << path << ": " << coGetLastOSErrorMessage());
		return false;
	}

	return true;
}

void coFileAccess::OnImplDestruct()
{
	HANDLE& handle = static_cast<HANDLE&>(impl);
	if (handle != INVALID_HANDLE_VALUE)
	{
		const BOOL res = ::CloseHandle(handle);
		if (res == FALSE)
		{
			coERROR("Failed to close the mapped file: " << coGetLastOSErrorMessage());
		}
		handle = INVALID_HANDLE_VALUE;
	}
}

void coFileAccess::OnImplConstruct()
{
	impl = INVALID_HANDLE_VALUE;
}

coResult coFileAccess::Write(const coArray<coByte>& buffer)
{
	return Write(buffer.data, buffer.count);
}

coResult coFileAccess::Write(const void* buffer, coUint32 size)
{
	HANDLE& handle = static_cast<HANDLE&>(impl);
	coTRY(handle != INVALID_HANDLE_VALUE, nullptr);
	DWORD writtenSize8 = 0;
	const BOOL res = ::WriteFile(handle, buffer, size, &writtenSize8, nullptr);
	if (res == FALSE)
	{
		coERROR("Failed to write to the file " << GetDebugName() << ": " << coGetLastOSErrorMessage());
		return false;
	}
	coTRY(size == writtenSize8, nullptr);
	return true;
}

coResult coFileAccess::Read(coArray<coByte>& buffer)
{
	HANDLE& handle = static_cast<HANDLE&>(impl);
	coTRY(handle != INVALID_HANDLE_VALUE, nullptr);
	DWORD readSize8 = 0;
	const BOOL res = ::ReadFile(handle, buffer.data, buffer.count, &readSize8, nullptr);
	if (res == FALSE)
	{
		coERROR("Failed to read from the file " << GetDebugName() << ": " << coGetLastOSErrorMessage());
		return false;
	}
	return readSize8 == buffer.count;
}

coResult coFileAccess::Flush()
{
	coTRY(mode == Mode::write, nullptr);
	HANDLE& handle = static_cast<HANDLE&>(impl);
	const BOOL res = ::FlushFileBuffers(handle);
	if (res == FALSE)
	{
		coERROR("Failed to flush the file " << GetDebugName() << ": " << coGetLastOSErrorMessage());
		return false;
	}
	return true;
}
