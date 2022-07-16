// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coSystem.h"
#include "coEntity.h"
#include "coComponent.h"
#include "component/coComponent_f.h"
#include "processor/coProcessor.h"
#include "processor/coProcessorConfig.h"
#include <math/transform/coTransform.h>

class Transform : public coComponent
{
	coDECLARE_COMPONENT(Transform, coComponent);
public:
	coTransform transform;
};

class Collider : public coComponent
{
	coDECLARE_COMPONENT(Collider, coComponent);
public:
};

coDEFINE_COMPONENT(Transform)
{
	return true;
}

coDEFINE_COMPONENT(Collider)
{
	return true;
}

void coTestEntities()
{
	coEntity* entity = new coEntity();

	Transform* transform = new Transform();
	transform->LinkTo(*entity);

	Collider* collider = new Collider();
	collider->LinkTo(*transform);
}
