// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

template <class T>
coFORCE_INLINE T* coBegin(coArray<T>& _a) { return _a.data; }
template <class T>
coFORCE_INLINE T* coEnd(coArray<T>& _a) { return _a.data + _a.count; }

// STD compat
template <class T>
coFORCE_INLINE T* begin(coArray<T> _a) { return coBegin(_a); }
template <class T>
coFORCE_INLINE T* end(coArray<T> _a) { return coEnd(_a); }
