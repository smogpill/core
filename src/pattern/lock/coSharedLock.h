// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

template <class T>
class [[nodiscard] coSharedLock final
{
public:
    explicit coSharedLock(T & m) : _mutex(m) { _mutex.LockShared(); }
    coSharedLock(const coScopedLock&) = delete;
    ~coSharedLock() { _mutex.UnlockShared(); }
    coSharedLock& operator=(const coSharedLock&) = delete;
private:
    T& _mutex;
};
