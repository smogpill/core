// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../entity/coEntityHandle.h"
#include <container/array/coDynamicArray.h>
class coEntityProcessor;

class coEntityContext
{
public:
	coEntityHandle CreateEntity();
	void DestroyEntity(const coEntityHandle&);
	void SetEntityEnabled(const coEntityHandle&, coBool);

private:
	coDynamicArray<coEntityProcessor*> processors;
};
