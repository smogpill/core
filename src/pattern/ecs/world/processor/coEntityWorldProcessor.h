// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../../coECSConfig.h"
#include "../../component/coComponentTypeHandle.h"
#include <container/array/coDynamicArray.h>
class coEntityProcessor;
class coEntityBatch;

class coEntityWorldProcessor
{
public:
	coEntityProcessor* processor = nullptr;
	coComponentTypeHandle componentTypes[co_maxNbComponentsPerProcessor];
	coDynamicArray<coEntityTypeID> entityTypes;
};
