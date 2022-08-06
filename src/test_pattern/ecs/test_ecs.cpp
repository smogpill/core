// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_pattern/pch.h"
#include "test/unit/coTest.h"
#include "pattern/ecs/component/coComponent.h"
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

class TestProcessorAB : public coEntityProcessor
{
	coDECLARE_BASE(coEntityProcessor);
public:
	void OnUpdate(const coEntityArray& array) override
	{
		Base::OnUpdate(array);
		TestAComp* comps_a = static_cast<TestAComp*>(array.components[0]);
		TestBComp* comps_b = static_cast<TestBComp*>(array.components[1]);
		for (coUint entityIdx = 0; entityIdx < array.nbEntities; ++entityIdx)
		{
			TestAComp& a = comps_a[entityIdx];
			TestBComp& b = comps_b[entityIdx];
		}
	}
};

coDEFINE_COMPONENT(TestAComp)
{
}

coDEFINE_COMPONENT(TestBComp)
{
}

coTEST(ecs, InitReflection)
{
	coTypeRegistry::CreateInstanceIfMissing();
}

coTEST(ecs, SimpleCase)
{
	TestProcessorAB processorAB;
	coEntityWorld world;
	world.AddProcessor(processorAB);
	coEntityHandle entity = world.CreateEntity();
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
