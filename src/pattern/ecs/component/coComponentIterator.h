// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coComponent.h"
#include <container/array/coFixedArray_f.h>

/*
class coComponentIterator
{
public:
	coComponentIterator(coComponent* comp = nullptr) : comp(comp) {}
	void operator++() { coASSERT(comp); comp = comp->GetNextComponent(); }
	coComponent& operator*() const { coASSERT(comp); return *comp; }
	coBool operator==(const coComponentIterator& it) const { return it.comp == comp; }
	coBool operator!=(const coComponentIterator& it) const { return it.comp != comp; }
private:
	coComponent* comp = nullptr;
};

class coComponentConstIterator
{
public:
	coComponentConstIterator(const coComponent* comp = nullptr) : comp(comp) {}
	void operator++() { coASSERT(comp); comp = comp->GetNextComponent(); }
	const coComponent& operator*() const { coASSERT(comp); return *comp; }
	coBool operator==(const coComponentConstIterator& it) const { return it.comp == comp; }
	coBool operator!=(const coComponentConstIterator& it) const { return it.comp != comp; }
private:
	const coComponent* comp = nullptr;
};

class coLinkedComponents
{
public:
	coLinkedComponents(const coComponent* comp) : comp(comp) {}
	coComponentIterator begin() const { return coComponentIterator(const_cast<coComponent*>(comp)); }
	coComponentConstIterator cbegin() const { return coComponentConstIterator(comp); }
	coComponentIterator end() const { return coComponentIterator(const_cast<coComponent*>(comp)); }
	coComponentConstIterator cend() const { return coComponentConstIterator(comp); }
private:
	const coComponent* comp = nullptr;
};

class coComponentReverseIterator
{
public:
	coComponentReverseIterator(coComponent** comp = nullptr) : comp(comp) {}
	void operator++() { coASSERT(comp); --comp; }
	coComponent& operator*() { coASSERT(comp && *comp); return **comp; }
	coBool operator==(const coComponentReverseIterator& it) const { return it.comp == comp; }
	coBool operator!=(const coComponentReverseIterator& it) const { return it.comp != comp; }
private:
	coComponent** comp = nullptr;
};

class coReversedLinkedComponents
{
public:
	coReversedLinkedComponents(coComponent* comp);
	coComponentReverseIterator begin() const { return coComponentReverseIterator(components.data + components.count - 1); }
	coComponentReverseIterator end() const { return coComponentReverseIterator(components.data-1); }
private:
	coFixedArray<coComponent*, 256> components;
};

template <class Functor>
coBool coVisitAll(coComponent* component, Functor func)
{
	if (component)
	{
		coComponent* it = component;
		do
		{
			if (!func(*it))
				return false;
			it = it->GetNextComponent();
		} while (it != component);
	}
	return true;
}

template <class Functor>
coBool coReverseVisitAll(coComponent* component, Functor func)
{
	if (component)
	{
		coFixedArray<coComponent*, 256> components;
		auto stack = [&](coComponent& comp)
		{
			coPushBack(components, &comp);
			return true;
		};
		coVisitAll(component, stack);

		while (components.count)
		{
			coComponent* comp = coPopBack(components);
			if (!func(*comp))
				return false;
		}
	}
	return true;
}
*/