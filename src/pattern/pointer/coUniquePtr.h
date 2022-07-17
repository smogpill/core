// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

template <class T>
class coUniquePtr
{
public:
    coUniquePtr(){}
    coUniquePtr(coNullPtr){}
    coUniquePtr(T* ptr) : ptr(ptr) {}
	coUniquePtr(const coUniquePtr&) = delete;
    coUniquePtr(coUniquePtr&& other) : ptr(other.ptr) { other.ptr = nullptr; }
    ~coUniquePtr() { delete ptr; }
	coUniquePtr& operator=(const coUniquePtr&) = delete;
    void operator=(T* p) { if (p != ptr) { delete ptr; ptr = p; } }
    [[nodiscard]] T& operator*() const { coASSERT(ptr); return *ptr; }
    [[nodiscard]] T* operator->() const { return ptr; }
    explicit operator bool() const { return static_cast<bool>(ptr); }
    T* Release() { T* p = ptr; ptr = nullptr; return p; }
    T* Get() const { return ptr; }
    void Reset() { delete ptr; ptr = nullptr; }

private:
    T* ptr = nullptr;
};
