// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../coECSConfig.h"
class coEntityHandle;
class coComponent;

class coEntityArray
{
public:
	coEntityHandle* entities = nullptr;
	coComponent* components[co_maxNbComponentsPerProcessor];
	coUint32 nbEntities = 0;
};
