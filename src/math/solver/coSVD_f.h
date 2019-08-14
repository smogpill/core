// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coVec4;
class coMat4;

void coSolveBySymmetricSVD(coVec4& out, const coMat4& SymA, const coVec4& b);
