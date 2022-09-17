// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coTransformComp.h"
#include <math/transform/coTransform_f.h>
#include "../../coECS.h"
#include "../../component/coComponent_f.h"

coBEGIN_COMPONENT(coTransformComp);
coEND_COMPONENT();

void coTransformComp::SetLocal(const coTransform& t)
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

void coTransformComp::SetGlobal(const coTransform& t)
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

void coTransformComp::SetParent(const coEntityHandle& newParent)
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

void coTransformComp::UpdateLocal()
{
	local = global * coInverse(GetParentGlobal());
	localDirty = 0;
}

void coTransformComp::UpdateGlobal()
{
	global = local * GetParentGlobal();
	globalDirty = 0;
}

const coTransform& coTransformComp::GetParentGlobal() const
{
	const coECS* ecs = coECS::instance;
	coTransformComp* parentTransform = ecs->GetComponent<coTransformComp>(parent);
	return parentTransform->GetGlobal();
}

void coTransformProcessor::OnInit(coProcessorInfo& info)
{
	Base::OnInit(info);
}

void coTransformProcessor::OnStart(coProcessorInfo& info)
{
	Base::OnStart(info);
}

void coTransformProcessor::OnStop(coProcessorInfo& info)
{
	Base::OnStop(info);
}

void coTransformProcessor::OnShutdown(coProcessorInfo& info)
{
	Base::OnShutdown(info);
}

void coTransformUpdateProcessor::Update()
{

}
