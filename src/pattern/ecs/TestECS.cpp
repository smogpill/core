// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coSystem.h"
#include "coEntity.h"
#include "coComponent.h"
#include "processor/coProcessor.h"
#include "processor/coProcessorConfig.h"
#include <math/transform/coTransform.h>
#include "lang/reflect/coTypeDecl_f.h"

class Transform : public coComponent
{
	coDECLARE_SUPER(coComponent);
	coDECLARE_REFLECTED_VIRTUAL();
public:
	coTransform transform;
};

class Collider : public coComponent
{
	coDECLARE_SUPER(coComponent);
	coDECLARE_REFLECTED_VIRTUAL();
public:
};

coDEFINE_TYPE(Transform)
{
	return true;
}

coDEFINE_TYPE(Collider)
{
	return true;
}

void coTestEntities()
{
	coProcessorConfig config;

	coEntity* entity = new coEntity();

	Transform* transform = new Transform();
	transform->LinkTo(*entity);

	Collider collider = new Collider();
	collider->LinkTo(*transform);
}
