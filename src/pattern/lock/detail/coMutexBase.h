// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <debug/log/coAssert.h>

template <class T>
class _coMutexBase
{
public:
	static constexpr coBool s_recursive = std::is_same<T, std::recursive_mutex>::value;
	void Lock()
	{
#ifdef coDEV
		if (!TryLock())
		{
			_impl.lock();
			_lockedThreadID = std::this_thread::get_id();
			++_nbRecursiveLocks;
		}
#else
		_impl.lock();
#endif
	}
	void Unlock()
	{
		coASSERT(_lockedThreadID == std::this_thread::get_id());
#ifdef coDEV
		if (!--_nbRecursiveLocks)
			_lockedThreadID = std::thread::id();
#endif
		_impl.unlock();
	}

	[[nodiscard]] coBool TryLock()
	{
#ifdef coDEV
		coASSERT(_lockedThreadID != std::this_thread::get_id() || s_recursive);
		if (_impl.try_lock())
		{
			_lockedThreadID = std::this_thread::get_id();
			++_nbRecursiveLocks;
			return true;
		}
		return false;
#else
		return _impl.try_lock();
#endif
	}

#ifdef coDEV
	coBool LockedByMe() const
	{
		return _lockedThreadID == std::this_thread::get_id();
	}
#endif

protected:
	T _impl;
#ifdef coDEV
	std::thread::id _lockedThreadID;
	coUint32 _nbRecursiveLocks = 0;
#endif
};
