// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "ecs/pch.h"
#include "coNode.h"
#include <math/transform/coTransform_f.h>
#include "../../coECS.h"
#include "../../component/coComponent_f.h"

void coNode::Init(coNode& comp)
{
	
}

coBEGIN_COMPONENT(coNode);
coDEFINE_COMPONENT_INIT();
coEND_COMPONENT();

void coNode::SetLocal(const coTransform& t)
{
	local = t;
	localDirty = 0;
	if (parent.IsValid())
	{
		globalDirty = 1;
	}
	else
	{
		global = t;
	}
	++version;
}

void coNode::SetGlobal(const coTransform& t)
{
	global = t;
	globalDirty = 0;
	if (parent.IsValid())
	{
		localDirty = 1;
	}
	else
	{
		local = t;
	}
	++version;
}

void coNode::SetParent(const coEntityHandle& newParent)
{
	if (parent != newParent)
	{
		parent = newParent;
		if (parent.IsValid())
		{
			localDirty = 1;
		}
		else
		{
			local = global;
		}
	}
}

void coNode::UpdateLocal()
{
	local = global * coInverse(GetParentGlobal());
	localDirty = 0;
}

void coNode::UpdateGlobal()
{
	global = local * GetParentGlobal();
	globalDirty = 0;
}

const coTransform& coNode::GetParentGlobal() const
{
	const coECS* ecs = coECS::instance;
	coNode* parentTransform = ecs->GetComponent<coNode>(parent);
	return parentTransform->GetGlobal();
}
