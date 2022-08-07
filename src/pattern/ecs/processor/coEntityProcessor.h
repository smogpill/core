// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../component/coComponentTypeHandle.h"
class coEntityBatch;
class coEntityArray;
class coType;

class coEntityProcessor
{
public:
	virtual ~coEntityProcessor() {}

	void AddComponentType(const coType& type);
	template <class T>
	void AddComponentType() { AddComponentType(*T::GetStaticType()); }
	coBool HasComponentType(const coType& type) const;
	template <class T>
	coBool HasComponentType() const { return HasComponentType(*T::GetStaticType()); }

	virtual void OnUpdate(const coEntityBatch& batch);
	virtual void OnUpdate(const coEntityArray& array) = 0;

protected:
	coUint8 nbComponentTypes = 0;
	coComponentTypeHandle componentTypeHandles[co_maxNbComponentsPerProcessor];
};
