// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../component/coComponentTypeHandle.h"
class coEntityBatch;
class coEntityArray;
class coComponentMask;
class coType;

class coEntityProcessor
{
public:
	virtual ~coEntityProcessor() {}

	template <class T>
	void AddComponentTypeR() { AddComponentType(*T::GetStaticType()); }
	template <class T>
	void AddComponentTypeRW() { AddComponentTypeRW(*T::GetStaticType()); }
	coBool HasComponentType(const coType& type) const;
	template <class T>
	coBool HasComponentType() const { return HasComponentType(*T::GetStaticType()); }
	coUint GetNbComponentTypes() const { return nbComponentTypes; }
	const coComponentTypeHandle* GetComponentTypeArray() const { return componentTypeHandles; }
	coBool IsCompatible(const coComponentMask&) const;


	virtual void OnUpdate(const coEntityBatch& batch);
	virtual void OnUpdate(const coEntityArray& array) = 0;

protected:
	void AddComponentType(const coType& type);
	void AddComponentTypeRW(const coType& type);
	friend class coEntityWorld;
	template <class T>
	const T* GetComponentsR(const coEntityArray& array, coUint index) const { return static_cast<const T*>(array.components[index]); }
	template <class T>
	T* GetComponentsRW(const coEntityArray& array, coUint index) const { return static_cast<T*>(array.components[index]); }

	coUint8 nbComponentTypes = 0;
	coComponentTypeHandle componentTypeHandles[co_maxNbComponentsPerProcessor];
	coUint32 componentTypesWriteAccess = 0;
};
