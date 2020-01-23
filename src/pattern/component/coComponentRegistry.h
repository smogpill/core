// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "container/map/coHashMap.h"

class coComponent;
class coEntityHandle;

class coComponentRegistry
{
public:
	coComponent* Get(const coEntityHandle& _entityHandle) const;

private:
	coHashMap<coComponent*, 4096> entityTocomponentMap;
};
