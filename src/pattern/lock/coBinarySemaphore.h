// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coBinarySemaphore
{
public:
	coFORCE_INLINE void Acquire() { impl.acquire(); }
	coFORCE_INLINE coBool TryAcquire() { return impl.try_acquire(); }
	coFORCE_INLINE void Release() { impl.release(); }
private:
	std::binary_semaphore _impl;
};
