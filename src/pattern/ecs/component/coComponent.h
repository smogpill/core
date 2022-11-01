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
	coDECLARE_CLASS_NO_POLYMORPHISM(_Class_)

class coComponent
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coComponent);
public:
	template <class T> T* GetComponent() const;

protected:
	friend class coEntity;

	static void Init(coComponent&) {}
	static void Shutdown(coComponent&) {}
	static void Start(coComponent&) {}
	static void Stop(coComponent&) {}
private:
};
