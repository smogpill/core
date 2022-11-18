// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
class coDCEL;

void coDissolveDegenerateFace(coDCEL&, coUint32 anyHalfEdgeIdx);
void coRemoveDegenerateFaces(coDCEL&);