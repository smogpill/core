// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coComponent.h"
#include "coEntity.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coTypeDecl_f.h"

coDEFINE_CLASS(coComponent)
{
	coDEFINE_FIELD(nextComponent)
	{

	}
}

coComponent::coComponent()
{
	nextComponent = this;
}
