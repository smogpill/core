// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/string/coDynamicString.h>

coBool coLastOSErrorExists();
void coClearLastOSError();
coDynamicString coGetLastOSErrorMessage();
coDynamicString coGetOSErrorMessage(coUint errorValue);
coUint64 coGetTimeStamp();
coFloat64 coGetSeconds(coUint64 timeStamp);