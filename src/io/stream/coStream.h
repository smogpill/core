// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coStreamBuffer;

class coStream : public coObject
{
	coDECLARE_BASE(coObject);
public:
	const coResult& GetResult() const { return result; }

protected:
	virtual void SetErrorMode();

	coResult result = true;
};
 