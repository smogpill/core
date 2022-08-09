// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_pattern/pch.h"
#include "test/unit/coTest.h"
#include "pattern/ecs/component/coComponent.h"
#include "pattern/ecs/component/coComponentMask.h"
#include "pattern/ecs/component/coComponent_f.h"
#include "pattern/ecs/processor/coEntityProcessor.h"
#include "pattern/ecs/entity/coEntityArray.h"
#include "pattern/ecs/world/coEntityWorld.h"
#include "pattern/pointer/coUniquePtr.h"
#include <io/archive/coArchive.h>
#include <math/transform/coTransform.h>
#include <lang/reflect/coTypeRegistry.h>

class TestAComp : public coComponent
{
	coDECLARE_COMPONENT(TestAComp, coComponent);
public:
};

class TestBComp : public coComponent
{
	coDECLARE_COMPONENT(TestBComp, coComponent);
public:
};

class TestCComp : public coComponent
{
	coDECLARE_COMPONENT(TestCComp, coComponent);
public:
};

coDEFINE_COMPONENT(TestAComp)
{
}

coDEFINE_COMPONENT(TestBComp)
{
}

coDEFINE_COMPONENT(TestCComp)
{
}

class TestProcessorAC : public coEntityProcessor
{
public:
	TestProcessorAC()
	{
		AddComponentType<TestAComp>();
		AddComponentType<TestCComp>();
	}
	void OnUpdate(const coEntityArray& array) override
	{
		auto* as = GetComponents<TestAComp>(array, 0);
		auto* cs = GetComponents<TestCComp>(array, 1);
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

	coComponentMask mask;
	mask.Add<TestAComp>();
	mask.Add<TestBComp>();
	coEntityHandle entity = world.CreateEntity(mask);
	world.Update();
	world.DestroyEntity(entity);
}

coTEST(ecs, Prefab)
{
}

coTEST(ecs, Archive)
{
}

coTEST(ecs, Children)
{

}
