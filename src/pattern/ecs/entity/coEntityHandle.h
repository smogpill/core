// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coEntityHandle
{
public:
	void operator=(const coEntityHandle& h) { static_cast<coUint64&>(*this) = h.GetRaw(); }
	coBool operator==(const coEntityHandle& h) { return GetRaw() == h.GetRaw(); }
	coBool operator!=(const coEntityHandle& h) { return GetRaw() != h.GetRaw(); }
	coUint64 GetRaw() const { return *static_cast<coUint64*>(this); }

	coUint32 index = coUint32(-1);
	coUint32 generation = coUint32(-1);
};
