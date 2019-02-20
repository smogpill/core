// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include <io/stream/coStream.h>

class coOutputStream : public coStream
{
	coDECLARE_SUPER(coStream);
public:
	~coOutputStream();
	void Write(coByte value);
	void Write(const void* data, coUint size8);

protected:
	coDynamicArray<coByte*> blocks;
};
