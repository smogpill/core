// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

template <class T> coFORCE_INLINE auto begin(T& _this) -> decltype(coBegin(_this)) { return coBegin(_this); }
template <class T> coFORCE_INLINE auto begin(const T& _this) -> decltype(coBegin(_this)) { return coBegin(_this); }
template <class T> coFORCE_INLINE auto end(T& _this) -> decltype(coEnd(_this)) { return coEnd(_this); }
template <class T> coFORCE_INLINE auto end(const T& _this) -> decltype(coEnd(_this)) { return coEnd(_this); }
