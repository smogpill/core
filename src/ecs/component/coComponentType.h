// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/reflect/coType.h>
#include <container/array/coDynamicArray.h>
class coType;
class coComponent;
class coEntity;

using coComponentFunc = void (*)(coEntity&, coComponent&);

class coComponentType : public coCustomTypeData
{
public:
	void AddDependency(const coType&);
	template <class T>
	void AddDependency() { AddDependency(*T::GetStaticType()); }

	coComponentFunc initFunc = nullptr;
	coComponentFunc shutdownFunc = nullptr;
	coComponentFunc startFunc = nullptr;
	coComponentFunc stopFunc = nullptr;

	coDynamicArray<coType*> dependencies;
};
