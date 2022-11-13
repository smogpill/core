// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../../component/coComponent.h"
#include "../../entity/coEntityHandle.h"

class coOwnership : public coComponent
{
	coDECLARE_COMPONENT(coOwnership, coComponent);
public:
	coEntityHandle firstChild;
	coEntityHandle previousSibling;
	coEntityHandle nextSibling;
};
