// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#define coPROJECT_NAME_STRING coSTRINGIFY(coPROJECT_NAME)
#define coVERSION_STRING coSTRINGIFY(coVERSION_MAJOR) "." coSTRINGIFY(coVERSION_MINOR) "." coSTRINGIFY(coVERSION_BUILD)
#define coPROJECT_NAME_WITH_VERSION_STRING coPROJECT_NAME_STRING " " coVERSION_STRING
