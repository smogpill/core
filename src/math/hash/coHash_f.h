// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

coUint32 coHash32(coUint32 _val);
coUint32 coHash32(coUint64 _val);
coUint32 coHash32(const void* _data, coUint _len);
coUint64 coHash64(const void* _data, coUint _len);
coUint64 coHash64(coUint32 _val);
coUint64 coHash64(coUint64 _val);
