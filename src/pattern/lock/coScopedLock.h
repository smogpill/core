// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

template <class T>
class [[nodiscard]] coScopedLock final
{
public:
    explicit coScopedLock(T& m) : _mutex(m) { _mutex.Lock(); }
    coScopedLock(const coScopedLock&) = delete;
    ~coScopedLock() { _mutex.Unlock(); }
    coScopedLock& operator=(const coScopedLock&) = delete;
private:
    T& _mutex;
};
