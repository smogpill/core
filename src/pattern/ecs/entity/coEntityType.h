// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../component/coComponentMask.h"
#include <lang/reflect/coType.h>

class coEntityType : public coType
{
	coDECLARE_BASE(coType);
public:
	void AddComponent(const coType& type);
	template <class T>
	void AddComponent() { AddComponent(*T::GetStaticType()); }
	const coComponentMask& GetComponentMask() const { return componentMask; }

private:
	coComponentMaskHandle componentMaskHandle;
	coComponentMask componentMask;
};
