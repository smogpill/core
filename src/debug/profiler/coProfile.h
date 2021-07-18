// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#define coPROFILE_FRAME(_frameName_, ...) OPTICK_FRAME(_frameName_, __VA_ARGS__)
#define coPROFILE_THREAD(_threadName_) OPTICK_THREAD(_threadName_)
#define coPROFILE_EVENT(...) OPTICK_EVENT(__VA_ARGS__)
