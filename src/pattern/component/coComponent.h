// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "pattern/object/coObject.h"

class coEntity;

class coComponent : public coObject
{
	friend coEntity;
	coDECLARE_SUPER(coObject);
	coDECLARE_REFLECTED_VIRTUAL();
public:
	coComponent();

	coEntity* GetEntity() const { return entity; }

private:
	coEntity* entity;
};
