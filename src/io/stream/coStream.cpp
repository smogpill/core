// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/stream/coStream.h"
#include "lang/result/coResult_f.h"
#include "io/stream/coStreamBuffer.h"

coStream::coStream()
	: buffer(nullptr)
{

}

coStream::InitConfig::InitConfig()
	: buffer(nullptr)
{

}

coResult coStream::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);

	buffer = config.buffer;
	coTRY(buffer, nullptr);
	return true;
}

void coStream::Clear()
{
	if (buffer)
		buffer->Reset();
}

void coStream::Flush()
{
	if (buffer)
		buffer->Flush();
}

coResult coStream::GetResult() const
{
	return buffer && buffer->GetResult();
}
