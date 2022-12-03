// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/result/coResult.h>

class coImage
{
public:
	~coImage();

	void Clear();
	coResult Load(const coChar* path);
	coResult LoadOnlyHeader(const coChar* path);
	coBool IsEmpty() const;

	coUchar* buffer = nullptr;
	coUint width = 0;
	coUint height = 0;
	coUint nbComponents = 0;
};
