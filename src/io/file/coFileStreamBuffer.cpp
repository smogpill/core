// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/file/coFileStreamBuffer.h"
#include "io/file/coFileAccess.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coNumericLimits.h"

const coUint co_bufferSize8 = 65536;

coFileStreamBuffer::coFileStreamBuffer()
	: mode(Mode::read)
	, impl(nullptr)
{
	OnImplConstruct();
}

coFileStreamBuffer::~coFileStreamBuffer()
{
	Flush();
	OnImplDestruct();
}

coFileStreamBuffer::InitConfig::InitConfig()
	: mode(Mode::read)
{

}

coResult coFileStreamBuffer::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);

	if (config.debugName == "")
		SetDebugName(config.path);

	mode = config.mode;

	coTRY(OnImplInit(config), nullptr);

	coResize(buffer, co_bufferSize8);

	windowBegin = buffer.data;
	windowEnd = coEnd(buffer);
	cursor = windowBegin;

	switch (mode)
	{
	case coFileAccess::Mode::read:
	{
		refill = static_cast<RefillFunc>(&coFileStreamBuffer::RefillRead);
	}
	break;
	case coFileAccess::Mode::write:
	{
		refill = static_cast<RefillFunc>(&coFileStreamBuffer::RefillWrite);
	}
	break;
	default:
	{
		coERROR("File access not supported for streaming: " << mode);
		SetErrorMode();
	}
	break;
	}

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
	if (mode == coFileAccess::Mode::write)
	{
		if (cursor != windowBegin)
		{
			coASSERT(cursor - windowBegin < coNumericLimits<coUint>::Max());
			const coUint remainingSize8 = static_cast<coUint>(cursor - windowBegin);
			result &= ImplWrite(buffer.data, remainingSize8);
			cursor = windowBegin;
		}
	}
}

coBool coFileStreamBuffer::RefillRead()
{
	cursor = windowBegin;
	coUint readSize8;
	if (ImplRead(readSize8, buffer.data, buffer.count))
	{
		windowEnd = windowBegin + readSize8;
		return true;
	}
	else
	{
		coERROR("Failed to read more data from the file " << GetDebugName());
		SetErrorMode();
		return false;
	}
}

coBool coFileStreamBuffer::RefillWrite()
{
	cursor = windowBegin;
	if (ImplWrite(buffer.data, buffer.count))
	{
		return true;
	}
	else
	{
		coERROR("Failed to write data to the file " << GetDebugName());
		SetErrorMode();
		return false;
	}
}
