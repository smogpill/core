// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../coECSConfig.h"
#include <math/scalar/coUint64_f.h>
class coType;
class coComponentTypeHandle;

class alignas(16) coComponentMask
{
public:
	void Add(const coType& type);
	template <class T>
	void Add() { Add(*T::GetStaticType()); }
	coBool HasType(const coComponentTypeHandle) const;

	coBool operator==(const coComponentMask& m) const;
	coBool operator!=(const coComponentMask& m) const { return !operator==(m); }
	coUint16 GetNbComponents() const;
	template <class F>
	void VisitTypes(F) const;

private:
	void Add(const coComponentTypeHandle handle);
	void Remove(const coComponentTypeHandle handle);

	static constexpr coUint nbMasks = co_maxNbComponentsPerWorld / 64;
	coUint64 masks[nbMasks] = {};
};

inline coBool coComponentMask::operator==(const coComponentMask& m) const
{
	for (coUint i = 0; i < nbMasks; ++i)
	{
		if (masks[i] != m.masks[i])
			return false;
	}
	return true;
}

template <class F>
void coComponentMask::VisitTypes(F func) const
{
	coUint offset = 0;
	for (const coUint64 mask : masks)
	{
		if (mask)
		{
			for (coUint i = 0; i < 64; ++i)
			{
				if ((mask >> i) & 1ull)
				{
					func(coComponentTypeHandle(offset + i));
				}
			}
		}
		offset += 64;
	}
}
