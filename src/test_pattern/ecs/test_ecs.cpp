// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_pattern/pch.h"
#include "test/unit/coTest.h"
#include "pattern/ecs/component/coComponent.h"
#include "pattern/ecs/component/coComponentMask.h"
#include "pattern/ecs/component/coComponent_f.h"
#include "pattern/ecs/component/ownership/coOwnership.h"
#include "pattern/ecs/processor/coEntityProcessor.h"
#include "pattern/ecs/entity/coEntityArray.h"
#include "pattern/ecs/entity/coEntityTypeDecl.h"
#include "pattern/ecs/entity/coEntityTypeDecl_f.h"
#include "pattern/ecs/world/coEntityWorld.h"
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

class TransformC : public coComponent
{
	coDECLARE_COMPONENT(TransformC, coComponent)
public:
	coTransform local;
	coTransform world;
	coUint32 version = 0;
};

coDEFINE_COMPONENT(TestCComp)
{
	type->AddDependency<TestAComp>();
}

coDECLARE_ENTITY_TYPE(TestEntity);

coDEFINE_ENTITY_TYPE(TestEntity)
{
	type->AddComponent<coOwnership>();
	type->AddComponent<TestBComp>();
	type->AddComponent<TestCComp>();
}

class TestProcessorAC : public coEntityProcessor
{
public:
	TestProcessorAC()
	{
		AddComponentTypeRW<TestAComp>();
		AddComponentTypeRW<TestCComp>();
	}
	void OnUpdate(const coEntityArray& array) override
	{
		auto* as = GetComponentsRW<TestAComp>(array, 0);
		auto* cs = GetComponentsRW<TestCComp>(array, 1);
		for (coUint entityIdx = 0; entityIdx < array.nbEntities; ++entityIdx)
		{
			TestAComp& a = as[entityIdx];
			TestCComp& c = cs[entityIdx];
		}
	}
};

coTEST(ecs, InitReflection)
{
	coTypeRegistry::CreateInstanceIfMissing();
}

coTEST(ecs, SimpleCase)
{
	coEntityWorld world;
	TestProcessorAC processorAC;
	world.AddProcessor(processorAC);

	coEntityHandle entity = world.CreateEntity<TestEntity>();
	world.Update();
	world.DestroyEntity(entity);
}

coTEST(ecs, Prefab)
{
}

coTEST(ecs, Archive)
{
	coEntityWorld world;
	coArchive archive;

	// Save
	{
		const coEntityHandle entity = world.CreateEntity<TestEntity>();
		world.Save(entity, archive);
		world.DestroyEntity(entity);
	}
	
	// Load
	{
		const coEntityHandle entity = world.Load(archive);
		coEXPECT(entity.IsValid());
	}
}

coTEST(ecs, Children)
{
	coEntityWorld world;

	const coEntityHandle parent = world.CreateEntity<TestEntity>();
	const coEntityHandle childA = world.CreateEntity<TestEntity>();
	const coEntityHandle childB = world.CreateEntity<TestEntity>();
}
