// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coEntityHandle
{
public:
	coEntityHandle() = default;
	coEntityHandle(coUint32 index, coUint32 generation) : index(index), generation(generation) {}
	void operator=(const coEntityHandle& h) { reinterpret_cast<coUint64&>(index) = h.GetRaw(); }
	coBool operator==(const coEntityHandle& h) { return GetRaw() == h.GetRaw(); }
	coBool operator!=(const coEntityHandle& h) { return GetRaw() != h.GetRaw(); }
	coUint64 GetRaw() const { return reinterpret_cast<const coUint64&>(index); }

	coUint32 index = coUint32(-1);
	coUint32 generation = coUint32(-1);
};
