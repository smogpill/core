// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../../coECSConfig.h"
#include "../../component/coComponentTypeHandle.h"
#include <container/array/coDynamicArray.h>
class coEntityProcessor;
class coEntityBatch;
class coEntityContainer;
class coComponentMask;

class coEntityWorldProcessor
{
public:
	void RegisterEntityType(const coEntityPatternID& entityTypeID, const coEntityContainer& container, const coComponentMask& mask);
	coBool HasEntityType(const coEntityPatternID& entityTypeID) const;

	struct EntityTypeInfo
	{
		coEntityPatternID entityTypeID;
		coUint16 componentIndicesInEntityType[co_maxNbComponentsPerProcessor];
	};
	coEntityProcessor* processor = nullptr;
	coDynamicArray<EntityTypeInfo> entityTypeInfos;
};
