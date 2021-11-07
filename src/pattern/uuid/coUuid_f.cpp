// Copyright(c) 2020-2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coUuid_f.h"

#ifdef coMSWINDOWS
#pragma comment(lib, "rpcrt4.lib")
#endif

coUuid coMakeUuid()
{
#ifdef coMSWINDOWS
    UUID uuid;
    UuidCreate(&uuid);
    static_assert(sizeof(UUID) == sizeof(coUuid));
    return reinterpret_cast<const coUuid&>(uuid);
#else
#error missing impl
#endif
}

coUuid coMakeUuid(const coChar* str)
{
#ifdef coMSWINDOWS
    UUID uuid;
    UuidFromStringA((const RPC_CSTR)(str), &uuid);
    static_assert(sizeof(UUID) == sizeof(coUuid));
    return reinterpret_cast<const coUuid&>(uuid);
#else
#error missing impl
#endif
}
