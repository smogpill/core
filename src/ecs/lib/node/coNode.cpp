// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "ecs/pch.h"
#include "coNode.h"
#include <math/transform/coTransform_f.h>
#include "../../entity/coEntity_f.h"
#include "../../coECS.h"
#include "../../component/coComponent_f.h"

coBEGIN_COMPONENT(coNode);
coDEFINE_COMPONENT_START();
coDEFINE_COMPONENT_STOP();
coDEFINE_ON_DESERIALIZED();
coDEFINE_FIELD(local);
coEND_COMPONENT();

coBEGIN_ENTITY(coNodeEntity);
entity->AddComponent<coNode>();
coEND_ENTITY();

coNode::coNode()
	: version(0)
	, localDirty(false)
	, globalDirty(false)
	, static_(false)
	, started(false)
{
}

void coNode::OnDeserialized()
{
	Base::OnDeserialized();
	globalDirty = true;
}

void coNode::Init(coEntity& entity)
{
	Base::Init(entity);
}

void coNode::Start(coEntity& entity)
{
	Base::Start(entity);
	RefreshParentNode();
	started = 1;
}

void coNode::Stop(coEntity& entity)
{
	started = false;
	SetParentNode(nullptr);
	Base::Stop(entity);
}

void coNode::SetLocal(const coTransform& t)
{
	if (local != t)
	{
		local = t;
		localDirty = 0;
		++version;
		SetGlobalDirtyRec();
	}
}

void coNode::TranslateGlobal(const coVec3& translation)
{
	coTransform newGlobal = GetGlobal();
	newGlobal.translation += translation;
	SetGlobal(newGlobal);
}

void coNode::SetGlobal(const coTransform& t)
{
	if (global != t)
	{
		global = t;
		globalDirty = 0;
		++version;
		localDirty = 1;
		SetGlobalDirtyOnDescendents();
	}
}

void coNode::SetParent(const coEntityHandle& newParent)
{
	if (parent != newParent)
	{
		parent = newParent;
		if (started)
		{
			RefreshParentNode();
		}
	}
}

void coNode::SetLocalTranslation(const coVec3& translation)
{
	coTransform t = GetLocal();
	t.translation = translation;
	SetLocal(t);
}

void coNode::SetGlobalTranslation(const coVec3& translation)
{
	coTransform t = GetGlobal();
	t.translation = translation;
	SetGlobal(t);
}

void coNode::UpdateLocal() const
{
	if (parentNode)
		local = global * coInverse(parentNode->GetGlobal());
	else
		local = global;
	localDirty = 0;
}

void coNode::UpdateGlobal() const
{
	if (parentNode)
		global = local * parentNode->GetGlobal();
	else
		global = local;
	globalDirty = 0;
}

void coNode::SetGlobalDirtyRec()
{
	if (!globalDirty)
	{
		globalDirty = 1;
		SetGlobalDirtyOnDescendents();
	}
}

void coNode::SetGlobalDirtyOnDescendents()
{
	coNode* child = firstChild;
	while (child)
	{
		child->SetGlobalDirtyRec();
		child = child->nextSibling;
	}
}

void coNode::SetParentNode(coNode* node)
{
	if (parentNode != node)
	{
		if (parentNode)
		{
			if (nextSibling)
			{
				previousSibling->nextSibling = nextSibling;
				nextSibling->previousSibling = previousSibling;
			}
			if (parentNode->firstChild == this)
				parentNode->firstChild = nextSibling;
			parentNode = nullptr;
			previousSibling = nullptr;
			nextSibling = nullptr;
		}

		parentNode = node;

		if (parentNode)
		{
			coNode* parentFirstChild = parentNode->firstChild;
			if (parentFirstChild)
			{
				previousSibling = parentFirstChild->previousSibling;
				nextSibling = parentFirstChild;
				previousSibling->nextSibling = this;
				parentFirstChild->previousSibling = this;
			}
			else
			{
				parentNode->firstChild = this;
			}
		}
		localDirty = 1;
	}
}

void coNode::RefreshParentNode()
{
	coNode* node = coECS::instance->GetComponent<coNode>(parent);
	SetParentNode(node);
}
