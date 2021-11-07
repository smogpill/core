// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../singleton/coSingleton.h"
#include <container/array/coDynamicArray.h>
#include <lang/result/coResult.h>
class coEntity;
class coEntityHandle;
class coUuid;

class coEntitySystem
{
	coDECLARE_SINGLETON(coEntitySystem);
public:
	coEntitySystem();
	~coEntitySystem();
	const coEntityHandle& Give(coEntity& entity);
	coEntity* Get(const coEntityHandle& handle) const;
	const coEntityHandle& Get(const coUuid& uuid) const;
	coResult Init(const coArray<coEntityHandle>& entities);
	coResult Start(const coArray<coEntityHandle>& entities);
	void Stop(const coArray<coEntityHandle>& entities);
	void Release(const coArray<coEntityHandle>& entities);

private:
	coDynamicArray<coEntity*> entities;
};
