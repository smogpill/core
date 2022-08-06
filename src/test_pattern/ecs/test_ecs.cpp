// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_pattern/pch.h"
#include "test/unit/coTest.h"
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

coTEST(ecs, InitReflection)
{
	coTypeRegistry::CreateInstanceIfMissing();
}

coTEST(ecs, SimpleEntity)
{
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
