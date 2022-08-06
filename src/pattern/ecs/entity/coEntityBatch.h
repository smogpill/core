// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray.h>
#include "../coECSConfig.h"
#include "coEntityArray.h"

class coEntityBatch
{
public:
	coEntityArray arrays[co_maxNbComponentsPerProcessor];
	coUint32 nbArrays = 0;
};
