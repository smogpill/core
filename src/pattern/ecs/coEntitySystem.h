// Copyright(c) 2020-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coEntityHandle.h"
#include "../singleton/coSingleton.h"
#include <container/array/coDynamicArray.h>
#include <lang/result/coResult.h>
class coEntity;
class coUuid;
class coBinaryOutputStream;
class coBinaryInputStream;

class coEntitySystem
{
	coDECLARE_SINGLETON(coEntitySystem);
public:
	coEntitySystem();
	~coEntitySystem();
	void Give(coEntity& entity);
	coEntity* Get(const coUuid& uuid) const;

private:
	coDynamicArray<coEntity*> entities;
};
