// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../coComponent.h"
#include <container/array/coFixedArray.h>

class coComponentIterator
{
public:
	coComponentIterator(coComponent* comp = nullptr) : comp(comp) {}
	void operator++() { coASSERT(comp); comp = comp->GetNextComponent(); }
	coComponent& operator*() { coASSERT(comp); return *comp; }
	coBool operator==(const coComponentIterator& it) const { return it.comp == comp; }
	coBool operator!=(const coComponentIterator& it) const { return it.comp != comp; }
private:
	coComponent* comp = nullptr;
};

class coLinkedComponents
{
public:
	coLinkedComponents(coComponent* comp) : it(comp) {}
	coComponentIterator begin() const { return it; }
	coComponentIterator end() const { return it; }
private:
	coComponentIterator it = nullptr;
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
