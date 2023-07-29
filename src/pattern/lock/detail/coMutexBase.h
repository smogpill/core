// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

template <class T>
class _coMutexBase
{
public:
	void Lock()
	{
#ifdef coDEV
		if (!TryLock())
		{
			_impl::lock();
			_lockedThreadID = std::this_thread::get_id();
		}
#else
		_impl.lock();
#endif
	}
	void Unlock()
	{
		coASSERT(_lockedThreadID == std::this_thread::get_id());
#ifdef coDEV
		_lockedThreadID = thread::id();
#endif
		_impl.unlock();
	}

	[[nodiscard]] coBool TryLock()
	{
#ifdef coDEV
		coASSERT(_lockedThreadID != std::this_thread::get_id());
		if (_impl.try_lock())
		{
			_lockedThreadID = std::this_thread::get_id();
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
#endif
};
