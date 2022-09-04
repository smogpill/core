// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coEntityHandle
{
public:
	coEntityHandle();
	coEntityHandle(const coEntityHandle&) = default;
	coEntityHandle(coUint32 world, coUint32 index, coUint32 generation) : index(index), world(world), generation(generation) {}
	coEntityHandle& operator=(const coEntityHandle& h) = default;
	coBool operator==(const coEntityHandle& h) { return GetRaw() == h.GetRaw(); }
	coBool operator!=(const coEntityHandle& h) { return GetRaw() != h.GetRaw(); }
	coBool IsValid() const { return index != coUint32(-1); }
	coBool IsGenerationMax() const { return generation == maxGeneration; }
	coUint64 GetRaw() const { return reinterpret_cast<const coUint64&>(index); }

	static const coEntityHandle invalid;
	static const coUint32 maxGeneration = 0x7fffffff;
	coUint32 index = coUint32(-1);
	coUint32 world : 1;
	coUint32 generation : 31;
};

inline coEntityHandle::coEntityHandle()
	: world(0)
	, generation(maxGeneration)
{

}