// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/path/coPath_f.h"
#include "io/path/coPathStatus.h"
#include "container/string/coDynamicString16.h"
#include "container/string/coDynamicString16_f.h"
#include "container/string/coConstString16.h"
#include "lang/result/coResult_f.h"
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
				coERROR("Path error: " << coGetOSErrorMessage(errval));
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

coResult coGetAbsolutePath(coDynamicString& _out, const coConstString& _this)
{
	coTRY(coIsPathNormalized(_this), nullptr);
	coDynamicString16 p;
	coSetFromUTF8(p, _this);
	coNullTerminate(p);
	coClear(_out);
	WCHAR buffer[MAX_PATH];
	const DWORD ret = ::GetFullPathNameW(p.data, coARRAY_SIZE(buffer), buffer, NULL);
	if (ret == 0)
	{
		coERROR("Path error for '" << _this << "': " << coGetLastOSErrorMessage());
		return false;
	}
	else if (ret >= coARRAY_SIZE(buffer))
	{
		coERROR("Path buffer is too small to retrieve the absolute path of: '" << _this << "'.");
		return false;
	}
	coSetFromWide(_out, buffer);
	coASSERT(coIsPathNormalized(_out));
	return true;
}
