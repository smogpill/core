// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/stream/coMemoryStreamBuffer.h"

void coMemoryStreamBuffer::Reset()
{
	Super::Reset();
	coWARN_NOT_AVAILABLE();
}

coResult coMemoryStreamBuffer::OnInit(const coObject::InitConfig& _config)
{
	Super::OnInit(_config);

	windowBegin = buffer.data;
	windowEnd = coEnd(buffer);
	cursor = windowEnd;
	coWARN_NOT_AVAILABLE();
	return true;
}

coBool coMemoryStreamBuffer::Refill()
{
	cursor = windowBegin;
	coWARN_NOT_AVAILABLE();
	return true;
}
