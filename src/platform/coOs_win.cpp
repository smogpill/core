// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "platform/pch.h"
#include "platform/coOs.h"
#include "container/string/coDynamicString_f.h"
#include "container/string/coConstString16.h"
#include "debug/log/coLog.h"

coBool coLastOsErrorExists()
{
	return ::GetLastError() != 0;
}

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
	LPWSTR str = nullptr;
	const DWORD ret = ::FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, _osError, 0, (LPTSTR)&str, 0, nullptr);

	if (ret == 0)
	{
		const DWORD err = ::GetLastError();
		coERROR("::FormatMessage() failed: error id " << (coUint)err);
	}

	coSetFromWide(_str, coConstString16(str));

	::LocalFree(str);
}

coUint64 coGetTimeStamp()
{
	LARGE_INTEGER largeInteger;
	const BOOL ret = QueryPerformanceCounter(&largeInteger);
	if (ret)
	{
		return largeInteger.QuadPart;
	}
	else
	{
		const DWORD err = ::GetLastError();
		coERROR("::QueryPerformanceCounter() failed: error id " << (coUint)err);
		return 0;
	}
}

coFloat64 coGetSeconds(coUint64 timeStamp)
{
	LARGE_INTEGER largeInteger;
	const BOOL ret = QueryPerformanceFrequency(&largeInteger);
	if (ret)
	{
		return timeStamp / coFloat64(largeInteger.QuadPart);
	}
	else
	{
		const DWORD err = ::GetLastError();
		coERROR("::QueryPerformanceFrequency() failed: error id " << (coUint)err);
		return 0.0f;
	}
}
