// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/file/coFileStreamBuffer.h"
#include "io/file/coFileAccess.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coNumericLimits.h"

coFileStreamBuffer::coFileStreamBuffer()
	: fileAccess(nullptr)
{

}

coFileStreamBuffer::~coFileStreamBuffer()
{
	Flush();
}

coResult coFileStreamBuffer::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);

	fileAccess = config.fileAccess;
	coTRY(fileAccess, nullptr);



	return true;
}

void coFileStreamBuffer::Reset()
{
	Super::Reset();
	cursor = windowBegin;
	coWARN_NOT_AVAILABLE();
}

void coFileStreamBuffer::Flush()
{
	Super::Flush();
	if (fileAccess)
	{
		const coFileAccess::Mode mode = fileAccess->GetMode();
		if (mode == coFileAccess::Mode::write)
		{
			coASSERT(cursor - windowBegin < coNumericLimits<coUint>::Max());
			const coUint remainingSize8 = static_cast<coUint>(cursor - windowBegin);
			result &= fileAccess->Write(buffer.data, remainingSize8);
			cursor = windowBegin;
		}

		result &= fileAccess->Flush();
	}
}

coBool coFileStreamBuffer::RefillRead()
{
	cursor = windowBegin;
	coUint readSize8;
	if (fileAccess->Read(readSize8, buffer.data, buffer.count))
	{
		windowEnd = windowBegin + readSize8;
		return true;
	}
	else
	{
		coERROR("Failed to read more data from the file " << fileAccess->GetDebugName());
		SetErrorMode();
		return false;
	}
}

coBool coFileStreamBuffer::RefillWrite()
{
	cursor = windowBegin;
	if (fileAccess->Write(buffer.data, buffer.count))
	{
		return true;
	}
	else
	{
		coERROR("Failed to write data to the file " << fileAccess->GetDebugName());
		SetErrorMode();
		return false;
	}
}
