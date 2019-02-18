// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/stream/coStream.h"
#include "lang/result/coResult_f.h"
#include "io/stream/coStreamBuffer.h"

coStream::coStream(coStreamBuffer& _buffer)
	: buffer(&_buffer)
{

}

void coStream::Clear()
{
	buffer->Reset();
}

void coStream::Flush()
{
	buffer->Flush();
}

coResult coStream::GetResult() const
{
	return buffer->GetResult();
}
