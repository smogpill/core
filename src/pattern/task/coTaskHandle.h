// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "../pointer/coReference.h"
#include "coTask.h"

class coTaskHandle : private coRef<coTask>
{
	using Base = coRef<coTask>;
public:
	coTaskHandle() = default;
	coTaskHandle(const coTaskHandle&) = default;
	coTaskHandle(coTaskHandle&& handle) : Base(std::move(handle)) { }

	coTaskHandle& operator= (const coTaskHandle& handle) { Base::operator=(handle); return *this; }
	coTaskHandle& operator= (coTaskHandle&& handle) noexcept { Base::operator=(std::move(handle)); return *this; }
	coBool IsValid() const { return GetPtr() != nullptr; }
	coBool IsDone() const { return GetPtr() != nullptr && GetPtr()->IsDone(); }
	void AddDependency(coUint nb = 1) const { GetPtr()->AddDependency(nb); }
	void RemoveDependency(coUint nb = 1) const { GetPtr()->RemoveDependencyAndQueue(nb); }
	static void	RemoveDependencies(coTaskHandle* handles, coUint nbHandles, coUint count = 1);
	coUint32 GetNbDependencies() const { return GetPtr()->GetNbDependencies(); }

private:
	friend class coTaskManager;
	friend class coTaskBarrier;
	explicit coTaskHandle(coTask* task) : Base(task) {}

	using Base::GetPtr;
};

#include "coTaskHandle.inl"
