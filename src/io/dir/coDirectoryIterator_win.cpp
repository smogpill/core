// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/dir/coDirectoryIterator.h"
#include "io/path/coPathStatus.h"
#include "io/path/coPath_f.h"
#include "lang/result/coResult_f.h"
#include "debug/log/coLog.h"
#include "platform/coOs.h"

void* coDirectoryIterator::GetDefaultImpl()
{
	return INVALID_HANDLE_VALUE;
}

coResult _coCloseDirectoryIteratorHandle(const HANDLE& _handle)
{
	if (_handle != INVALID_HANDLE_VALUE)
	{
		const BOOL ret = ::FindClose(_handle);
		if (ret == FALSE)
		{
			coDynamicString s;
			coDumpLastOsError(s);
			coERROR("Failed to close the directory handle: " << s);
			return false;
		}
	}
	return true;
}

void _coConvert(coDirectoryEntry& _entry, const WIN32_FIND_DATAA& _data)
{
	_entry.name = _data.cFileName;
	if (_data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
	{
		_entry.status.status = coPathStatus::Status::error;
	}
	else if (_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		_entry.status.status = coPathStatus::Status::directory;
	}
	else
	{
		_entry.status.status = coPathStatus::Status::regularFile;
	}
}

coBool coDirectoryIterator::operator== (const coDirectoryIterator& _) const
{
	return impl == _.impl;
}

coBool coDirectoryIterator::operator!= (const coDirectoryIterator& _) const
{
	return impl != _.impl;
}

coBool _coAcceptEntry(const coDirectoryEntry& _entry)
{
	return !coIsDotOrDoubleDot(_entry.name);
}

coResult coDirectoryIterator::OnImplInit(const InitConfig& _config)
{
	HANDLE& handle = static_cast<HANDLE&>(impl);
	if (_config.path.count == 0)
	{
		return true;
	}

	coDynamicString searchPath(_config.path);
	searchPath << "/*";
	coNullTerminate(searchPath);

	WIN32_FIND_DATAA data;
	handle = ::FindFirstFileA(searchPath.data, &data);
	if (handle == INVALID_HANDLE_VALUE)
	{
		const DWORD lastError = ::GetLastError();
		if (lastError == ERROR_FILE_NOT_FOUND || lastError == ERROR_NO_MORE_FILES)
		{
			// no error, just an empty directory
			return true;
		}
		else
		{
			coDynamicString s;
			coDumpOsError(lastError, s);
			coERROR("Failed to get a handle of the first file of the directory '" << _config.path << ": " << s);
			return false;
		}
	}

	_coConvert(entry, data);

	if (!_coAcceptEntry(entry))
	{
		operator++();
	}

	return true;
}

void coDirectoryIterator::OnImplConstruct()
{
	impl = INVALID_HANDLE_VALUE;
}

void coDirectoryIterator::OnImplDestruct()
{
	HANDLE& handle = static_cast<HANDLE&>(impl);
	if (handle != INVALID_HANDLE_VALUE)
	{
		coCHECK(_coCloseDirectoryIteratorHandle(handle), nullptr);
		handle = INVALID_HANDLE_VALUE;
	}
}

coDirectoryIterator& coDirectoryIterator::operator++ ()
{
	// Reset
	coClear(entry.name);
	entry.status = coPathStatus();
	HANDLE& handle = static_cast<HANDLE&>(impl);
	if (handle == INVALID_HANDLE_VALUE)
	{
		coERROR("Attempt to increment an end iterator");
		return *this;
	}

	for (;;)
	{
		// Based on Boost::filesystem::operations.cpp::dir_itr_increment()
		{
			WIN32_FIND_DATAA data;

			// Find next file
			if (::FindNextFileA(handle, &data) != 0)
			{
				_coConvert(entry, data);
			}
			else
			{
				const DWORD error = ::GetLastError();
				if (error != ERROR_NO_MORE_FILES)
				{
					coDynamicString s;
					coDumpOsError(error, s);
					coERROR("Failed to find the next file: " << s);
				}
				coCHECK(_coCloseDirectoryIteratorHandle(handle), nullptr);
				entry = coDirectoryEntry();
				handle = INVALID_HANDLE_VALUE;
				return *this;
			}
		}

		if (_coAcceptEntry(entry))
		{
			break;
		}
	}

	return *this;
}
