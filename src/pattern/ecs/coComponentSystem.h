// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coComponentHandle.h"
#include "../singleton/coSingleton.h"
#include <container/pool/coPool_f.h>
#include <lang/result/coResult.h>

class coEntityHandle;

template <class T>
class coComponentSystem
{
public:
	coCompHandle<T> Create();
	void Destroy(const coCompHandle<T>& handle);

protected:
	virtual coResult OnInit(const coEntityHandle& entityH, T& comp) {}
	virtual coResult OnStart(T& comp) {}
	virtual void OnStop(T& comp) {}
	virtual void OnRelease(T& comp) {}
	virtual void Write(T& comp, coBinaryOutputStream& stream) const {}
	virtual void Read(T& comp, coBinaryInputStream& stream) {}

private:
	using Pool = coPool<T, coCompHandle<T>::UnderlyingType>;
	Pool pool;
};

template<class T>
coCompHandle<T> coComponentSystem<T>::Create()
{
	return coCreate(pool);
}

template<class T>
void coComponentSystem<T>::Destroy(const coCompHandle<T>& handle)
{
	return coDestroy(pool, handle);
}
