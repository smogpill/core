// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/array/coFixedArray.h"

class coSignature
{
public:
	coSignature();

	coUint64 resultTypeId;
	coUint64 ownerTypeId;
	coFixedArray<coUint64, 8> paramsTypeIds;
};
