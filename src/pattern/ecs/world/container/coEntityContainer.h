// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../../component/coComponentMask.h"

class coEntityContainer
{
public:
	coUint32 CreateEntity(const coEntityHandle&) { return 0; }
	void DestroyEntity(coUint32) {}
	void Reserve(coUint32 nb) {}

private:
	friend class coEntityWorld;

	coComponentMask componentMask;
	coEntityHandle* entities = nullptr;
	void* buffer = nullptr;

	coUint32 nbEntities = 0;
	coUint32 nbReservedEntities = 0;
};
