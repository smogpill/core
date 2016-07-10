// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "platform/pch.h"
#include "platform/coOs.h"
#include "container/string/coDynamicString_f.h"
#include "debug/log/coLog.h"

void coClearLastOsError()
{
	::SetLastError(0);
}

void coDumpLastOsError(coDynamicString& _str)
{
	const DWORD err = ::GetLastError();
	if (err)
	{
		coDumpOsError(err, _str);
	}
}

void coDumpOsError(coUint _osError, coDynamicString& _str)
{
	LPSTR str = nullptr;
	const DWORD ret = ::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, _osError, 0, (LPTSTR)&str, 0, nullptr);

	if (ret == 0)
	{
		const DWORD err = ::GetLastError();
		coERROR("::FormatMessage() failed: error id " << (coUint)err);
	}

	_str = str;

	::LocalFree(str);
}
