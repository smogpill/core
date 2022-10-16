// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/reflect/coType.h>

class coEntityType : public coCustomTypeData
{
public:
	void AddComponent(const coType& type);
	template <class T>
	void AddComponent() { AddComponent(*T::GetStaticType()); }
	const coArray<const coType*>& GetComponentTypes() const { return componentTypes; }

private:
	coDynamicArray<const coType*> componentTypes;
};

#define coDECLARE_ENTITY(_Type_) \
	class _Type_ \
	{ \
		coDECLARE_CLASS_NO_POLYMORPHISM(_Type_);\
	}