// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../../../uuid/coUuid.h"
#include "../../component/coComponent.h"

class coUuidComp : public coComponent
{
	coDECLARE_COMPONENT(coUuidComp, coComponent);
public:
private:
	coUuid uuid;
};
