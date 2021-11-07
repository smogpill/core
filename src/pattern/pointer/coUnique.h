// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include <memory> // temp
template<class T> using coUnique = std::unique_ptr<T, std::default_delete<T>>;
