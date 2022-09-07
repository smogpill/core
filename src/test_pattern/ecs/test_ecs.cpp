// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_pattern/pch.h"
#include "test/unit/coTest.h"
#include "pattern/ecs/entity/coEntity.h"
#include "pattern/ecs/component/coComponent.h"
#include "pattern/ecs/component/coComponent_f.h"
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

coDEFINE_COMPONENT(TestAComp)
{
}

coDEFINE_COMPONENT(TestBComp)
{
}

coTEST(ecs, Reflection)
{
	coTypeRegistry::CreateInstanceIfMissing();
}

coTEST(ecs, SimpleEntity)
{
	coUniquePtr<coEntity> entity = new coEntity();

	TestAComp* a = new TestAComp();
	entity->Give(*a);

	TestBComp* b = new TestBComp();
	entity->Give(*b);
}

coTEST(ecs, Prefab)
{
	coUniquePtr<coEntity> prefab = new coEntity();
	{
		TestAComp* a = new TestAComp();
		prefab->Give(*a);

		TestBComp* b = new TestBComp();
		prefab->Give(*b);
	}

	//coUniquePtr<coEntity> entity = prebab->Clone();
}

coTEST(ecs, Archive)
{
	coUniquePtr<coEntity> in = new coEntity();
	{
		TestAComp* a = new TestAComp();
		in->Give(*a);

		TestBComp* b = new TestBComp();
		in->Give(*b);
	}

	coArchive archive;
	archive.WriteRoot(*in.Get());

	coUniquePtr<coEntity> out = archive.CreateObjects<coEntity>();
}

coTEST(ecs, Children)
{
	coUniquePtr<coEntity> p = new coEntity();

	coEntity* a = new coEntity();
	coEntity* b = new coEntity();
	a->SetParent(p.Get());
	b->SetParent(p.Get());


}
