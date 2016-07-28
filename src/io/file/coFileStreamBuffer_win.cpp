// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/file/coFileStreamBuffer.h"
#include "io/path/coPath_f.h"
#include "platform/coOs.h"
#include "lang/result/coResult_f.h"
#include "container/string/coDynamicString16.h"
#include "container/string/coDynamicString16_f.h"

void coFileStreamBuffer::OnImplConstruct()
{
	impl = INVALID_HANDLE_VALUE;
}

void coFileStreamBuffer::OnImplDestruct()
{
	HANDLE& handle = static_cast<HANDLE&>(impl);
	if (handle != INVALID_HANDLE_VALUE)
	{
		const BOOL res = ::CloseHandle(handle);
		if (res == FALSE)
		{
			coDynamicString str;
			coDumpLastOsError(str);
			coERROR("Failed to close the file: " << str);
		}
		handle = INVALID_HANDLE_VALUE;
	}
}

coResult coFileStreamBuffer::OnImplInit(const InitConfig& _config)
{
	HANDLE& handle = static_cast<HANDLE&>(impl);

	// Open mode
	DWORD openMode = 0;
	switch (mode)
	{
	case Mode::read: openMode = GENERIC_READ; break;
	case Mode::write: openMode = GENERIC_WRITE; break;
	default:
	{
		coERROR("File mode not supported: " << mode);
		return false;
	}
	}

	// Creation disposition
	DWORD creationDisposition = 0;
	{
		switch (mode)
		{
		case Mode::read:
		{
			if (coExists(_config.path))
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
		case Mode::write:
		{
			creationDisposition = CREATE_ALWAYS;
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
	const DWORD flagsAndAttributes = (mode == Mode::read) ? FILE_ATTRIBUTE_READONLY : FILE_ATTRIBUTE_TEMPORARY;

	coDynamicString16 p;
	coSetFromUTF8(p, _config.path);
	coNullTerminate(p);
	
	handle = ::CreateFileW(p.data, openMode, FILE_SHARE_READ, nullptr, creationDisposition, flagsAndAttributes, nullptr);
	if (handle == INVALID_HANDLE_VALUE)
	{
		coDynamicString str;
		coDumpLastOsError(str);
		coERROR("Failed to open the file: " << _config.path << " (" << str << ")");
		return false;
	}
	coClearLastOsError(); // CreateFile can generate errors even if it succeeds (by design).

	return true;
}

coResult coFileStreamBuffer::ImplWrite(const coByte* _data, coUint _size8)
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

coResult coFileStreamBuffer::ImplRead(coUint& _readSize8, coByte* _data, coUint _size8)
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

coResult coFileStreamBuffer::ImplFlush()
{
	coTRY(mode == Mode::write, nullptr);
	HANDLE& handle = static_cast<HANDLE&>(impl);
	const BOOL res = ::FlushFileBuffers(handle);
	if (res == FALSE)
	{
		coDynamicString s;
		coDumpLastOsError(s);
		coERROR("Failed to flush the file " << GetDebugName() << "(" << s << ")");
		return false;
	}
	return true;
}
