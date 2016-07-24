// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/path/coPath_f.h"
#include "io/path/coPathStatus.h"
#include "container/string/coDynamicString16.h"
#include "container/string/coDynamicString16_f.h"
#include "platform/coOs.h"

coResult coGetPathStatus(coPathStatus& _status, const coConstString& _path)
{
	_status = coPathStatus();

	if (_path.data == nullptr)
	{
		return true;
	}

	// Based on Boost::filesystem::operations.cpp::detail::status()
	{
		coDynamicString16 p;
		coSetFromUTF8(p, _path);
		coNullTerminate(p);
		const DWORD attributes = ::GetFileAttributesW(p.data);
		if (attributes == INVALID_FILE_ATTRIBUTES)
		{
			const DWORD errval = ::GetLastError();
			switch (errval)
			{
			case ERROR_FILE_NOT_FOUND: _status.status = coPathStatus::Status::notFound; return true;
			case ERROR_PATH_NOT_FOUND: _status.status = coPathStatus::Status::notFound; return true;
			default:
			{
				coDynamicString error;
				coDumpOsError(errval, error);
				coERROR("Path error: " << error);
				_status.status = coPathStatus::Status::error; return false;
			}
			}
		}

		// Reparse point handling
		if (attributes & FILE_ATTRIBUTE_REPARSE_POINT)
		{
			_status.status = coPathStatus::Status::reparseFile;
			return true;
		}

		_status.status = (attributes & FILE_ATTRIBUTE_DIRECTORY) ?
			coPathStatus::Status::directory : coPathStatus::Status::regularFile;
	}

	return true;
}
