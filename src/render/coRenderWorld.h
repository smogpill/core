// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coRenderEntity;

class coRenderWorld : public coObject
{
	coDECLARE_SUPER(coObject);
public:

	coResult Add(coRenderEntity& _entity);
	void Remove(coRenderEntity& _entity);

	const coArray<coRenderEntity*>& GetEntities() const { return entities; }

private:
	coDynamicArray<coRenderEntity*> entities;
};