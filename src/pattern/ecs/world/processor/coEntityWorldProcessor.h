// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../../coECSConfig.h"
#include "../../component/coComponentTypeHandle.h"
#include <container/array/coDynamicArray.h>
class coEntityProcessor;
class coEntityBatch;
class coComponentMask;

class coEntityWorldProcessor
{
public:
	void RegisterEntityType(const coEntityTypeID& entityTypeID, const coComponentMask& mask);
	coBool HasEntityType(const coEntityTypeID& entityTypeID) const;

	struct EntityTypeInfo
	{
		coEntityTypeID entityTypeID;
		coUint16 componentIndicesInEntityType[co_maxNbComponentsPerProcessor];
	};
	coEntityProcessor* processor = nullptr;
	coComponentTypeHandle componentTypes[co_maxNbComponentsPerProcessor];
	coDynamicArray<EntityTypeInfo> entityTypeInfos;
};
