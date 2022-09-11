// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_pattern/pch.h"
#include "test/unit/coTest.h"
#include "pattern/ecs/coECS.h"
#include "pattern/ecs/entity/coEntity.h"
#include "pattern/ecs/component/coComponent.h"
#include "pattern/ecs/component/coComponent_f.h"
#include "pattern/ecs/entity/coEntityType_f.h"
#include "pattern/pointer/coUniquePtr.h"
#include <io/archive/coArchive.h>
#include <math/transform/coTransform.h>
#include <lang/reflect/coTypeRegistry.h>

class TestAComp : public coComponent
{
	coDECLARE_COMPONENT(TestAComp, coComponent);
public:
	coInt a = 7;
};

coDEFINE_COMPONENT(TestAComp)
{
}

class TestBComp : public coComponent
{
	coDECLARE_COMPONENT(TestBComp, coComponent);
public:
	coFloat b = 23.0f;
};

coDEFINE_COMPONENT(TestBComp)
{
}

class TestCComp : public coComponent
{
	coDECLARE_COMPONENT(TestCComp, coComponent);
public:
	coUint c = 99;
};

coDEFINE_COMPONENT(TestCComp)
{
	type->AddDependency<TestAComp>();
}

coDECLARE_ENTITY_TYPE(TestEntity);

coDEFINE_ENTITY_TYPE(TestEntity)
{
	type->AddComponent<TestBComp>();
	type->AddComponent<TestCComp>();
}

coTEST(ecs, Reflection)
{
	coTypeRegistry::CreateInstanceIfMissing();
	coECS::CreateInstanceIfMissing();
}

coTEST(ecs, CreateDestroyEntity)
{
	coECS* ecs = coECS::instance;
	coEntityHandle entity = ecs->CreateEntity<TestEntity>();
	coEXPECT(entity.IsValid());
	ecs->DestroyEntity(entity);
}

coTEST(ecs, ModifyEntity)
{
	coECS* ecs = coECS::instance;
	coEntityHandle entity = ecs->CreateEntity<TestEntity>();
	TestBComp* b = ecs->GetComponent<TestBComp>(entity);
	b->b = 777.0f;
	ecs->DestroyEntity(entity);
}

coTEST(ecs, Prefab)
{
	coECS* ecs = coECS::instance;
	coEntityHandle prefab = ecs->CreateEntity<TestEntity>();
	coEntityHandle instance = ecs->Clone(prefab);
	//coUniquePtr<coEntity> entity = prebab->Clone();
	ecs->DestroyEntity(instance);
	ecs->DestroyEntity(prefab);
}

coTEST(ecs, Archive)
{
	coECS* ecs = coECS::instance;
	coEntityHandle entity = coECS::instance->CreateEntity<TestEntity>();

	coArchive archive;
	ecs->SaveEntity(archive, entity);
	ecs->DestroyEntity(entity);
	coEntityHandle loadedEntity = ecs->LoadEntity(archive);
	ecs->DestroyEntity(loadedEntity);
}

coTEST(ecs, Children)
{
	coECS* ecs = coECS::instance;
	coEntityHandle parent = ecs->CreateEntity<TestEntity>();
	coEntityHandle a = ecs->CreateEntity<TestEntity>();
	coEntityHandle b = ecs->CreateEntity<TestEntity>();

	ecs->SetParent(a, parent);
	ecs->SetParent(b, parent);

	ecs->DestroyEntity(parent);
	coEXPECT(!ecs->IsAlive(a));
	coEXPECT(!ecs->IsAlive(b));
}
