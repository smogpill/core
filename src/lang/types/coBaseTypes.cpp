// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "lang/pch.h"
#include "lang/types/coBaseTypes.h"
#include "lang/reflect/coTypeAutoRegistrator.h"
#include "lang/reflect/coType.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coTypeDecl_f.h"

#define coDEFINE_FUNDAMENTAL_TYPE(_Type_) coClassTypeAutoRegistrator<_Type_> co_typeAutoRegistrator_##_Type_()

coDEFINE_FUNDAMENTAL_TYPE(coBool);
coDEFINE_FUNDAMENTAL_TYPE(coInt8);
coDEFINE_FUNDAMENTAL_TYPE(coInt16);
coDEFINE_FUNDAMENTAL_TYPE(coInt32);
coDEFINE_FUNDAMENTAL_TYPE(coInt64);
coDEFINE_FUNDAMENTAL_TYPE(coUint8);
coDEFINE_FUNDAMENTAL_TYPE(coUint16);
coDEFINE_FUNDAMENTAL_TYPE(coUint32);
coDEFINE_FUNDAMENTAL_TYPE(coUint64);
coDEFINE_FUNDAMENTAL_TYPE(coFloat);
coDEFINE_FUNDAMENTAL_TYPE(coFloat64);
coDEFINE_FUNDAMENTAL_TYPE(coNullPtr);
