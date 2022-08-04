// Copyright(c) 2020-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/reflect/coTypeDecl.h>
#include <lang/result/coResult.h>
class coBinaryOutputStream;
class coBinaryInputStream;
class coEntity;

#define coDECLARE_COMPONENT(_Class_, _Base_) \
	coDECLARE_BASE(_Base_); \
	coDECLARE_CLASS(_Class_)

class coComponent
{
	coDECLARE_CLASS(coComponent);
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
	virtual void OnShutdown(coEntity& entity) {}

private:
	coComponent* nextComponent = nullptr;
};
