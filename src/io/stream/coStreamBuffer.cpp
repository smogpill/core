// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/stream/coStreamBuffer.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coNumericLimits.h"

coStreamBuffer::coStreamBuffer()
	: windowBegin(nullptr)
	, windowEnd(nullptr)
	, cursor(nullptr)
	, result(true)
	, refill(nullptr)
	, isConst(false)
{
#ifdef coDEBUG
	result.tested = true;
#endif
}

coResult coStreamBuffer::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);

	windowBegin = nullptr;
	windowEnd = nullptr;
	cursor = nullptr;
	refill = nullptr;
	windowPos = 0;
	return true;
}

coStreamBuffer::~coStreamBuffer()
{
	coASSERT(result.tested);
}

void coStreamBuffer::Reset()
{
	coASSERT(result.tested);
	result = true;
#ifdef coDEBUG
	result.tested = true;
#endif
}

void coStreamBuffer::Flush()
{

}

void coStreamBuffer::SetPos(coInt64 /*_pos*/)
{
	coWARN_NOT_AVAILABLE();
	result = false;
#ifdef coDEBUG
	result.tested = false;
#endif
}

coUint coStreamBuffer::Write(const coByte* _data, coUint _size8)
{
	const coUint desiredSize8 = _size8;
	do
	{
		if (cursor != windowEnd); // likely
		else
		{
			if (!(this->*refill)())
				return desiredSize8 - _size8;
		}
		coASSERT(windowEnd - cursor < coNumericLimits<coUint>::Max());
		const coUint available8 = static_cast<coUint>(windowEnd - cursor);
		const coUint writeSize8 = (available8 >= _size8) ? _size8 : available8;
		coMemCopy(cursor, _data, writeSize8);
		_size8 -= writeSize8;
		_data += writeSize8;
		cursor += writeSize8;
	} while (_size8);
	return desiredSize8;
}

coUint coStreamBuffer::Read(coByte* _data, coUint _size8)
{
	const coUint desiredSize8 = _size8;
	do
	{
		if (cursor != windowEnd); // likely
		else
		{
			if (!(this->*refill)())
				return desiredSize8 - _size8;
		}
		coASSERT(windowEnd - cursor < coNumericLimits<coUint>::Max());
		const coUint available8 = static_cast<coUint>(windowEnd - cursor);
		const coUint readSize8 = (available8 >= _size8) ? _size8 : available8;
		coMemCopy(_data, cursor, readSize8);
		_size8 -= readSize8;
		_data += readSize8;
		cursor += readSize8;
	} while (_size8);

	return desiredSize8;
}

void coStreamBuffer::SetErrorMode()
{
	cursor = windowBegin = windowEnd = nullptr;
	refill = static_cast<RefillFunc>(&coStreamBuffer::RefillError);
	result = false;
#ifdef coDEBUG
	result.tested = false;
#endif
}

coBool coStreamBuffer::RefillError()
{
	cursor = windowBegin;
	return false;
}
