// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntity.h"
#include "lang/result/coResult_f.h"
#include <lang/reflect/coTypeDecl_f.h>
#include <lang/reflect/coTypeRegistry.h>
#include <io/stream/coBinaryInputStream.h>
#include <io/stream/coBinaryOutputStream.h>
#include <io/archive/coArchive.h>
#include <container/array/coFixedArray_f.h>
#include "../../uuid/coUuid_f.h"
#include "../component/coComponentIterator.h"
#include "../component/coComponent.h"

class coEntityComponents
{
public:
	coDECLARE_CLASS_NO_POLYMORPHISM(coEntityComponents);
};

coDEFINE_CLASS(coEntityComponents)
{
	type->triviallyCopyable = false;
	type->writeArchiveFunc = [](coArchive& archive, const void* obj) -> coUint32
	{
		const coEntity* entity = static_cast<const coEntity*>(obj);
		const coUint32 nbComponents = entity->GetNbComponents();
		if (nbComponents == 0)
			return 0;
		const coUint32 index = archive.GetSize();
		archive.Write(nbComponents);
		auto& data = archive.GetData();
		const coUint32 offsetsIdx = archive.GetSize();
		archive.PushBytes(nbComponents * sizeof(coUint32));
		coUint32 itOffsetIdx = offsetsIdx;
		auto func = [&](coComponent& comp)
		{
			const coUint32 compIdx = archive.WriteObject(&comp, *comp.GetType());
			(coUint32&)(data[itOffsetIdx]) = compIdx - itOffsetIdx;
			itOffsetIdx += sizeof(coUint32);
			return true;
		};
		coVisitAll(entity->GetFirstComponent(), func);
		return index;
	};
	type->readArchiveFunc = [](const coArchive& archive, coUint32 idx, void* obj)
	{
		coEntity* entity = static_cast<coEntity*>(obj);
		const coUint32 nbComponents = archive.Get<coUint32>(idx);
		coUint32 itIdx = idx + sizeof(coUint32);
		for (coUint32 componentIdx = 0; componentIdx < nbComponents; ++componentIdx)
		{
			const coUint32 compOffsetIdx = archive.Get<coUint32>(itIdx);
			if (compOffsetIdx)
			{
				coComponent* component = archive.CreateObjects<coComponent>(itIdx + compOffsetIdx);
				if (component)
				{
					entity->Give(*component);
				}
			}
			itIdx += sizeof(coUint32);
		}
	};
}

class coEntityChildren
{
public:
	coDECLARE_CLASS_NO_POLYMORPHISM(coEntityComponents);
};

coDEFINE_CLASS(coEntityChildren)
{
	type->triviallyCopyable = false;
	type->writeArchiveFunc = [](coArchive& archive, const void* obj) -> coUint32
	{
		const coEntity* entity = static_cast<const coEntity*>(obj);
		const coUint32 nbChildren = entity->GetNbChildren();
		if (nbChildren == 0)
			return 0;
		const coUint32 index = archive.GetSize();
		archive.Write(nbChildren);
		auto& data = archive.GetData();
		const coUint32 offsetsIdx = archive.GetSize();
		archive.PushBytes(nbChildren * sizeof(coUint32));
		coUint32 itOffsetIdx = offsetsIdx;
		const coType* entityType = coEntity::GetStaticType();
		auto func = [&](coEntity& child)
		{
			const coUint32 childIdx = archive.WriteObject(&child, *entityType);
			(coUint32&)(data[itOffsetIdx]) = childIdx - itOffsetIdx;
			itOffsetIdx += sizeof(coUint32);
			return true;
		};
		entity->VisitChildren(func);
		return index;
	};
	type->readArchiveFunc = [](const coArchive& archive, coUint32 idx, void* obj)
	{
		coEntity* entity = static_cast<coEntity*>(obj);
		const coUint32 nbChildren = archive.Get<coUint32>(idx);
		coUint32 itIdx = idx + sizeof(coUint32);
		for (coUint32 childIdx = 0; childIdx < nbChildren; ++childIdx)
		{
			const coUint32 childOffsetIdx = archive.Get<coUint32>(itIdx);
			if (childOffsetIdx)
			{
				coEntity* child = archive.CreateObjects<coEntity>(itIdx + childOffsetIdx);
				if (child)
				{
					child->SetParent(entity);
				}
			}
			itIdx += sizeof(coUint32);
		}
	};
}

coDEFINE_CLASS(coEntity)
{
	coDEFINE_VIRTUAL_FIELD(components, coEntityComponents);
	coDEFINE_VIRTUAL_FIELD(entities, coEntityChildren);
}

coEntity::coEntity()
{
	previousSibling = this;
	nextSibling = this;
	static coUint64 uuidGenerator = 0;
}

coEntity::~coEntity()
{
	coCHECK(SetState(State::NONE), nullptr);

	// Delete children
	{
		auto func = [&](coEntity& entity)
		{
			delete& entity;
			return true;
		};
		ReverseVisitChildren(func);
	}

	// Delete components
	{
		auto func = [&](coComponent& comp)
		{
			delete& comp;
			return true;
		};
		coReverseVisitAll(firstComponent, func);
	}
}

coResult coEntity::SetState(State newState)
{
	while (state != newState)
	{
		coTRY(TransitToNextState(newState), nullptr);
	}
	return true;
}

coResult coEntity::TransitToNextState(State targetState)
{
	// Should be more generic, like using a table of transitions

	switch (state)
	{
	case State::NONE:
	{
		switch (targetState)
		{
		case State::NONE: break;
		case State::INITIALIZED:
		case State::STARTED:
		{
			coTRY(Init(), nullptr);
			state = State::INITIALIZED;
			break;
		}
		default:
		{
			coASSERT(false);
			break;
		}
		}
		break;
	}
	case State::INITIALIZED:
	{
		switch (targetState)
		{
		case State::NONE:
		{
			Shutdown();
			state = State::NONE;
			break;
		}
		case State::INITIALIZED: break;
		case State::STARTED:
		{
			coTRY(Init(), nullptr);
			state = State::INITIALIZED;
			break;
		}
		default:
		{
			coASSERT(false);
			break;
		}
		}
		break;
	}
	case State::STARTED:
	{
		switch (targetState)
		{
		case State::NONE:
		case State::INITIALIZED:
		{
			Stop();
			state = State::INITIALIZED;
			break;
		}
		case State::STARTED: break;
		default:
		{
			coASSERT(false);
			break;
		}
		}
		break;
	}
	default:
	{
		coASSERT(false);
		break;
	}
	}
	return true;
}

coEntity* coEntity::Clone() const
{
	coASSERT(GetState() == State::NONE);
	coEntity* entity = new coEntity();
	coASSERT(false);
	return entity;
}

coUint coEntity::GetNbComponents() const
{
	coUint nb = 0;
	auto func = [&](coComponent& comp)
	{
		++nb;
		return true;
	};
	coVisitAll(firstComponent, func);
	return nb;
}

coUint coEntity::GetNbChildren() const
{
	coUint nb = 0;
	auto func = [&nb](coEntity& entity)
	{
		++nb;
		return true;
	};
	VisitChildren(func);
	return nb;
}

coResult coEntity::Init()
{
	// Init components
	{
		auto func = [&](coComponent& comp)
		{
			return comp.OnInit(*this);
		};
		if (!coVisitAll(firstComponent, func))
			return false;
	}

	// Init children
	{
		auto func = [this](coEntity& entity)
		{
			return entity.Init();
		};
		if (!VisitChildren(func))
			return false;
	}
	return true;
}

coResult coEntity::Start()
{
	// Start components
	{
		auto func = [this](coComponent& comp)
		{
			return comp.OnStart(*this);
		};
		if (!coVisitAll(firstComponent, func))
			return false;
	}

	// Start children
	{
		auto func = [this](coEntity& entity)
		{
			return entity.Start();
		};
		if (!VisitChildren(func))
			return false;
	}
	return true;
}

void coEntity::Stop()
{
	// Stop children
	{
		auto func = [this](coEntity& entity)
		{
			entity.Stop();
			if (entity.GetParentStateWhenAttached() == State::STARTED)
				delete &entity;
			return true;
		};
		ReverseVisitChildren(func);
	}

	// Stop components
	{
		auto func = [&](coComponent& comp)
		{
			comp.OnStop(*this);
			return true;
		};
		coReverseVisitAll(firstComponent, func);
	}
}

void coEntity::Shutdown()
{
	// Shutdown children
	{
		auto func = [this](coEntity& entity)
		{
			entity.Shutdown();
			if (entity.GetParentStateWhenAttached() == State::INITIALIZED)
				delete &entity;
			return true;
		};
		ReverseVisitChildren(func);
	}

	// Shutdown components
	{
		auto func = [&](coComponent& comp)
		{
			comp.OnShutdown(*this);
			return true;
		};
		coReverseVisitAll(firstComponent, func);
	}
}

void coEntity::Give(coComponent& component)
{
	if (firstComponent)
	{
		coComponent* last = firstComponent;
		while (last->nextComponent != firstComponent)
			last = last->nextComponent;
		last->nextComponent = &component;
		component.nextComponent = firstComponent;
	}
	else
	{
		firstComponent = &component;
	}
}

void coEntity::SetParent(coEntity* newParent)
{
	if (parent)
	{
		if (parent->firstChild == this)
		{
			if (nextSibling == this)
				parent->firstChild = nullptr;
			else
				parent->firstChild = nextSibling;
		}
		nextSibling->previousSibling = previousSibling;
		previousSibling->nextSibling = nextSibling;
		parentStateWhenAttached = State::NONE;
	}
	parent = newParent;
	if (newParent)
	{
		coEntity* parentFirstChild = newParent->firstChild;
		if (parentFirstChild)
		{
			parentFirstChild->previousSibling->nextSibling = this;
			previousSibling = parentFirstChild->previousSibling;

			parentFirstChild->previousSibling = this;
			nextSibling = parentFirstChild;
		}
		else
		{
			newParent->firstChild = this;
		}
		parentStateWhenAttached = newParent->GetState();
	}
}

void coEntity::Give(coEntity& entity)
{
	coASSERT(entity.parent == nullptr);
	if (firstChild)
	{
		firstChild->previousSibling->nextSibling = &entity;
		entity.previousSibling = firstChild->previousSibling;

		firstChild->previousSibling = &entity;
		entity.nextSibling = firstChild;
	}
	else
	{
		coASSERT(entity.previousSibling == nullptr);
		coASSERT(entity.nextSibling == nullptr);
		firstChild = &entity;
	}
	entity.parent = this;
}

coComponent* coEntity::GetComponent(const coType& type) const
{
	coComponent* foundComp = nullptr;
	auto func = [&](coComponent& comp)
	{
		if (comp.GetType() == &type)
		{
			foundComp = &comp;
			return false;
		}
		return true;
	};
	coVisitAll(firstComponent, func);
	return foundComp;
}