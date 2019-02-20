// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include <io/stream/coStream.h>

class coOutputStream : public coStream
{
	coDECLARE_SUPER(coStream);
public:
protected:
	void Write(coByte _v);
	void Write(const coByte* _data, coUint _size8);

	coDynamicArray<coByte*> blocks;
};
