// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../component/coComponent.h"
#include "../../uuid/coUuid.h"

class coUuidComp : public coComponent
{
	coDECLARE_COMPONENT(coUuidComp, coComponent);
public:
	coUuid uuid;
};
