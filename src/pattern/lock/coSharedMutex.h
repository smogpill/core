// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "detail/coMutexBase.h"

class coSharedMutex final : public _coMutexBase<std::shared_mutex>
{
public:
	coSharedMutex() = default;
	coSharedMutex(const coSharedMutex&) = delete;
	coSharedMutex& operator=(const coSharedMutex&) = delete;
	void LockShared()
	{
#ifdef coDEV
		++s_nbLockShared;
#endif
		_impl.lock_shared();
	}
	void UnlockShared()
	{
		_impl.unlock_shared();
#ifdef coDEV
		coASSERT(s_nbLockShared);
		--s_nbLockShared;
#endif
	}
	[[nodiscard]] coBool TryLockShared()
	{
#ifdef coDEV
		if (_impl.try_lock_shared())
		{
			++s_nbLockShared;
			return true;
		}
		return false;
#else
		return _impl.try_lock_shared();
#endif
	}

#ifdef coDEV
	coBool LockedSharedByMe() const
	{
		return s_nbLockShared != 0;
	}
#endif

private:
#ifdef coDEV
	thread_local static coUint32 s_nbLockShared = 0;
#endif
};
