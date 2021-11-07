// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/reflect/coTypeDecl.h>
#include <lang/result/coResult_f.h>
#include "coEntity.h"
class coBinaryOutputStream;
class coBinaryInputStream;
class coEntity;
class coEntityHandle;

class coComponent
{
	coDECLARE_REFLECTED_VIRTUAL();
public:
	virtual void Write(coBinaryOutputStream& stream) const {}
	virtual void Read(coBinaryInputStream& stream) {}
	coResult Init();
	void Release();
	coResult Start();
	void Stop();
	coEntity* GetEntity() const { return entity; }
	const coEntityHandle& GetEntityHandle() const;
	template <class T>
	T* GetComponent() const { coASSERT(entity); return entity->GetComponent<T>(); }

protected:
	virtual coResult OnInit() { return true; }
	virtual void OnRelease() {}
	virtual coResult OnStart() { return true; }
	virtual void OnStop() {}

private:
	friend class coEntity;
	coEntity* entity = nullptr;
};
