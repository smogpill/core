// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coEntityType.h"

#define coDECLARE_ENTITY_TYPE(_Type_) \
	class _Type_ : public coEntityType \
	{ \
	public:\
		static coEntityType* GetStaticType(); \
	protected:\
		static void OnInitType(coEntityType*); \
	}