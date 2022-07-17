// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_pattern/pch.h"
#include "test/unit/coTest.h"
#include "pattern/ecs/coEntity.h"
#include "pattern/ecs/coComponent.h"
#include "pattern/ecs/component/coComponent_f.h"
#include "pattern/pointer/coUniquePtr.h"
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

coTEST(ecs, SimpleEntity)
{
	coUniquePtr<coEntity> entity = new coEntity();

	Transform* transform = new Transform();
	entity->Give(*transform);

	Collider* collider = new Collider();
	entity->Give(*collider);
}
