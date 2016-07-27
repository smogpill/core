// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coDynamicString;

coBool coLastOsErrorExists();
void coClearLastOsError();
void coDumpLastOsError(coDynamicString& _str);
void coDumpOsError(coUint _osError, coDynamicString& _str);
