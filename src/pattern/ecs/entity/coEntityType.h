// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../component/coComponentMask.h"
#include <lang/reflect/coSymbol.h>

class coEntityType : public coSymbol
{
	coDECLARE_BASE(coSymbol);
public:
	coEntityType();
	void AddComponent(const coType& type);
	template <class T>
	void AddComponent() { AddComponent(*T::GetStaticType()); }
	const coComponentMask& GetComponentMask() const { return componentMask; }

private:
	coComponentMaskHandle componentMaskHandle;
	coComponentMask componentMask;
};
