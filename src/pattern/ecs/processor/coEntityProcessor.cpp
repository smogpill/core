// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntityProcessor.h"
#include "../entity/coEntityBatch.h"

void coEntityProcessor::OnUpdate(const coEntityBatch& batch)
{
	for (const coEntityArray& array : batch.arrays)
	{
		OnUpdate(array);
	}
}
