// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include <io/stream/coStream.h>

class coInputStream : public coStream
{
	coDECLARE_SUPER(coStream);
public:
	void Read(coByte& _v);
	void Read(coByte* _data, coUint _size8);

	const coArray<coByte> buffer;
};
