// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

// Almost a direct copy of Jorrit Rouwe's JoltPhysics' Reference.h

template <class T> class coRef;
template <class T> class coRefConst;

template <class T>
class coRefCounted
{
public:
	coRefCounted() = default;
	coRefCounted(const coRefCounted&) {}
	~coRefCounted();

	/// Disables the automatic deletion when all refs are removed, delete should be called manually
	void TakeOwnership() const { coASSERT(_nbRefs.fetch_add(s_owned, std::memory_order_relaxed) < s_owned); }

	coRefCounted& operator= (const coRefCounted&) { return *this; }
	coUint32 GetNbRefs() const { return _nbRefs.load(std::memory_order_relaxed); }
	void AddRef() const { _nbRefs.fetch_add(1, std::memory_order_relaxed); }
	void Release() const;

protected:
	static constexpr coUint32 s_owned = 0x0ebedded;
	mutable std::atomic<coUint32> _nbRefs = 0;
};

class coRefCountedVirtual
{
public:
	virtual ~coRefCountedVirtual() = default;
	virtual void AddRef() = 0;
	virtual void Release() = 0;
};

/// Class for automatic referencing, this is the equivalent of a pointer to type T
/// if you assign a value to this class it will increment the reference count by one
/// of this object, and if you assign something else it will decrease the reference
/// count of the first object again. If it reaches a reference count of zero it will
/// be deleted
template <class T>
class coRef
{
public:
	coRef() = default;
	coRef(T* ptr) : _ptr(ptr) { AddRef(); }
	coRef(const coRef<T>& ref) : _ptr(ref._ptr) { AddRef(); }
	coRef(coRef<T>&& ref) noexcept : _ptr(ref._ptr) { ref._ptr = nullptr; }
	~coRef() { Release(); }

	coRef<T>& operator= (T* p) { if (_ptr != p) { Release(); _ptr = p; AddRef(); } return *this; }
	coRef<T>& operator= (const coRef<T>& r) { if (_ptr != r._ptr) { Release(); _ptr = r._ptr; AddRef(); } return *this; }
	coRef<T>& operator= (coRef<T>&& r) noexcept { if (_ptr != r._ptr) { Release(); _ptr = r._ptr; r._ptr = nullptr; } return *this; }
	operator T* const () const { return _ptr; }
	operator T* () { return _ptr; }
	T* const operator -> () const { return _ptr; }
	T* operator -> () { return _ptr; }
	T& operator * () const { return *_ptr; }

	coBool operator==(const T* inRHS) const { return _ptr == inRHS; }
	coBool operator==(const coRef<T>& inRHS) const { return _ptr == inRHS._ptr; }
	coBool operator!=(const T* inRHS) const { return _ptr != inRHS; }
	coBool operator!=(const coRef<T>& inRHS) const { return _ptr != inRHS._ptr; }

	T* GetPtr() const { return _ptr; }
	T* GetPtr() { return _ptr; }

private:
	template <class T2> friend class RefConst;

	void AddRef() { if (_ptr) _ptr->AddRef(); }
	void Release() { if (_ptr) _ptr->Release(); }

	T* _ptr = nullptr;
};

/// Class for automatic referencing, this is the equivalent of a CONST pointer to type T
/// if you assign a value to this class it will increment the reference count by one
/// of this object, and if you assign something else it will decrease the reference
/// count of the first object again. If it reaches a reference count of zero it will
/// be deleted
template <class T>
class coRefConst
{
public:
	coRefConst() = default;
	coRefConst(const T* p) : _ptr(p) { AddRef(); }
	coRefConst(const coRefConst<T>& r) : _ptr(r._ptr) { AddRef(); }
	coRefConst(coRefConst<T>&& r) noexcept : _ptr(r._ptr) { r._ptr = nullptr; }
	coRefConst(const coRef<T>& r) : _ptr(r._ptr) { AddRef(); }
	coRefConst(coRef<T>&& r) noexcept : _ptr(r._ptr) { r._ptr = nullptr; }
	~coRefConst() { Release(); }
	coRefConst<T>& operator = (const T* r) { if (_ptr != r) { Release(); _ptr = r; AddRef(); } return *this; }
	coRefConst<T>& operator = (const coRefConst<T>& r) { if (_ptr != r._ptr) { Release(); _ptr = r._ptr; AddRef(); } return *this; }
	coRefConst<T>& operator = (coRefConst<T>&& r) noexcept { if (_ptr != r._ptr) { Release(); _ptr = r._ptr; r._ptr = nullptr; } return *this; }
	coRefConst<T>& operator = (const coRef<T>& r) { if (_ptr != r._ptr) { Release(); _ptr = r._ptr; AddRef(); } return *this; }
	coRefConst<T>& operator = (coRef<T>&& r) noexcept { if (_ptr != r._ptr) { Release(); _ptr = r._ptr; r._ptr = nullptr; } return *this; }

	operator const T* () const { return _ptr; }
	const T* operator -> () const { return _ptr; }
	const T& operator * () const { return *_ptr; }
	coBool operator == (const T* p) const { return _ptr == p; }
	coBool operator == (const coRefConst<T>& r) const { return _ptr == r._ptr; }
	coBool operator == (const coRef<T>& r) const { return _ptr == r._ptr; }
	coBool operator != (const T* p) const { return _ptr != p; }
	coBool operator != (const coRefConst<T>& r) const { return _ptr != r._ptr; }
	coBool operator != (const coRef<T>& r) const { return _ptr != r._ptr; }
	const T* GetPtr() const { return _ptr; }

private:
	/// Use "variable = nullptr;" to release an object, do not call these functions
	void AddRef() { if (_ptr) _ptr->AddRef(); }
	void Release() { if (_ptr) _ptr->Release(); }

	const T* _ptr = nullptr;
};

template <class T>
coRefCounted<T>::~coRefCounted()
{
#ifdef coDEV
	const coUint32 value = _nbRefs.load(std::memory_order_relaxed);
	coASSERT(value == 0 || value == s_owned);
#endif
}

template <class T>
void coRefCounted<T>::Release() const
{
	// Releasing a reference must use release semantics...
	if (_nbRefs.fetch_sub(1, std::memory_order_release) == 1)
	{
		// ... so that we can use aquire to ensure that we see any updates from other threads that released a ref before deleting the object
		std::atomic_thread_fence(std::memory_order_acquire);
		delete static_cast<const T*>(this);
	}
}
