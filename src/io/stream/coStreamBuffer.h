// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coStreamBuffer : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	coStreamBuffer();
	virtual ~coStreamBuffer();

	coResult GetResult() const { return result; }
	virtual void Reset();
	virtual void Flush();

	virtual void SetPos(coInt64 _pos);
	coInt64 GetPos() const;

	void SetErrorMode();

	coUint Write(coByte _v);
	coUint Write(const coByte* _data, coUint _size8);

	coUint Read(coByte& _v);
	coUint Read(coByte* _data, coUint _size8);

	coByte* begin() const { return cursor; }
	const coByte* cbegin() const { return cursor; }
	coByte* end() const { return windowEnd; }
	const coByte* cend() const { return windowEnd; }

	coBool Refill();

protected:
	typedef coBool(coStreamBuffer::*RefillFunc) ();

	coBool RefillError();

	virtual coResult OnInit(const coObject::InitConfig& _config) override;

	coByte* windowBegin;
	coByte* windowEnd;
	coByte* cursor;
	RefillFunc refill;
	coResult result;
	coInt64 windowPos;
	coBool isConst;
};

inline coBool coStreamBuffer::Refill()
{
	return (this->*refill)();
}

inline coInt64 coStreamBuffer::GetPos() const
{
	return windowPos + (cursor - windowBegin);
}

inline coUint coStreamBuffer::Write(coByte _v)
{
	if (cursor != windowEnd); // likely
	else
	{
		if (!(this->*refill)())
			return 0;
	}
	*cursor = _v;
	++cursor;
	return 1;
}

inline coUint coStreamBuffer::Read(coByte& _v)
{
	if (cursor != windowEnd); // likely
	else
	{
		if (!(this->*refill)())
		{
			_v = 0; // good for string buffers;
			return 0;
		}
	}
	_v = *cursor;
	++cursor;
	return 1;
}
