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

	coResult GetResult() const;
	virtual void Reset();
	virtual void Flush();
	virtual void SetPos(coInt64 _pos);
	coInt64 GetPos() const;
	void SetErrorMode();
	coUint Write(coByte _v);
	coUint Write(const coByte* _data, coUint _size8);
	coUint Read(coByte& _v);
	coUint Read(coByte* _data, coUint _size8);
	coBool Refill();

protected:
	typedef coBool (coStreamBuffer::*RefillFunc) ();
	virtual coResult OnInit(const coObject::InitConfig& _config) override;

	coByte* windowBegin = nullptr;
	coByte* windowEnd = nullptr;
	coByte* cursor = nullptr;
	RefillFunc refill = nullptr;
	coResult result = true;
	coInt64 windowPos = 0;
	coBool isConst = false;

private:
	coBool RefillError();
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
	if (coUNLIKELY(cursor == windowEnd))
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
	if (coUNLIKELY(cursor == windowEnd))
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
