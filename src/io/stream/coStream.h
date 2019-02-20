// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coStreamBuffer;

class coStream : public coObject
{
	coDECLARE_SUPER(coObject);
public:
	coResult GetResult() const { return result; }
	void SetPos(coUint32 pos);
	coUint32 GetPos() const { return pos; }

protected:
	void SetErrorMode();

	coResult result = true;
	coUint32 pos = 0;
};
 