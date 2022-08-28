// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntityType.h"

void coEntityType::AddComponent(const coType& type)
{
	componentMask.Add(type);
}
