// Copyright(c) 2020-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/reflect/coTypeDecl.h>
#include <lang/result/coResult.h>
class coBinaryOutputStream;
class coBinaryInputStream;
class coEntity;

#define coDECLARE_COMPONENT(_Class_, _Super_) \
	coDECLARE_SUPER(_Super_); \
	coDECLARE_REFLECTED_VIRTUAL()

class coComponent
{
	coDECLARE_REFLECTED_VIRTUAL();
public:
	coComponent();

	coComponent* GetNextComponent() const { return nextComponent; }
	template <class T> T* GetComponent() const;

	virtual void Write(coBinaryOutputStream& stream) const {}
	virtual void Read(coBinaryInputStream& stream) {}

protected:
	friend class coEntity;
	
	void SetNextComponent(coComponent* next) { nextComponent = next; }
	virtual coResult OnInit(coEntity& entity) { return true; }
	virtual coResult OnStart(coEntity& entity) { return true; }
	virtual void OnStop(coEntity& entity) {}
	virtual void OnRelease(coEntity& entity) {}

private:
	coComponent* nextComponent = nullptr;
};
