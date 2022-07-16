// Copyright(c) 2020-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/reflect/coTypeDecl.h>
#include <lang/result/coResult.h>
class coBinaryOutputStream;
class coBinaryInputStream;
class coEntity;

class coComponent
{
	coDECLARE_REFLECTED_VIRTUAL();
public:
	coComponent();
	~coComponent();

	virtual void Write(coBinaryOutputStream& stream) const;
	virtual void Read(coBinaryInputStream& stream);
	void LinkTo(coComponent& comp);
	coComponent* GetNextComponent() const { return nextComponent; }
	template <class T> T* GetComponent() const;

	coResult Init(coEntity& entity);
	coResult Start(coEntity& entity);
	void Stop(coEntity& entity);
	void Release(coEntity& entity);

protected:
	virtual coResult OnInit(coEntity& entity) { return true; }
	virtual coResult OnStart(coEntity& entity) { return true; }
	virtual void OnStop(coEntity& entity) {}
	virtual void OnRelease(coEntity& entity) {}

private:
	coComponent* nextComponent = nullptr;
};

template <class T>
T* coComponent::GetComponent() const
{
	const coType* type = T::GetStaticType();
	for (coComponent* comp = nextComponent; comp != this; comp = comp->nextComponent)
	{
		if (comp->GetType() == type)
			return static_cast<T*>(comp);
	}
	return nullptr;
}
