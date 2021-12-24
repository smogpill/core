// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "math/matrix/coMat3_f.h"

coMat3 coGetPolarDecompositionRotation(const coMat3& M, coFloat tolerance)
{
    // Implementation from https://github.com/Scrawk/Position-Based-Dynamics/blob/master/Assets/Common/LinearAlgebra/Matrix3x3fDecomposition.cs (MIT license)
    // https://github.com/brainexcerpts/3x3_polar_decomposition/blob/master/src/polar_decomposition_3x3.inl (BSD 2 license)

    coMat3 Mt = coTranspose(M);
    coFloat Mone = coOneNorm(M).x;
    coFloat Minf = coInfNorm(M).x;
    coFloat Eone;
    coMat3 MadjTt;
    coMat3 Et;
    
    const coFloat eps = 1.0e-15f;
    
    do
    {
        coSetRow(MadjTt, 0, coCross(coGetRow(Mt, 1), coGetRow(Mt, 2)));
        coSetRow(MadjTt, 1, coCross(coGetRow(Mt, 2), coGetRow(Mt, 0)));
        coSetRow(MadjTt, 2, coCross(coGetRow(Mt, 0), coGetRow(Mt, 1)));
    
        coFloat det = Mt.c0.x * MadjTt.c0.x + Mt.c1.x * MadjTt.c1.x + Mt.c2.x * MadjTt.c2.x;
    
        if (coAbs(det) < eps)
        {
            coUint index = coUint(-1);
            for (coUint i = 0; i < 3; i++)
            {
                coFloat len = coSquareLength(coGetRow(MadjTt, i)).x;
                if (len > eps)
                {
                    // index of valid cross product
                    // => is also the index of the vector in Mt that must be exchanged
                    index = i;
                    break;
                }
            }
    
            if (index == coUint(-1))
            {
                return coMat3(1.0f);
            }
            else
            {
    
                coSetRow(Mt, index, coCross(coGetRow(Mt, (index + 1) % 3), coGetRow(Mt, (index + 2) % 3)));
                coSetRow(MadjTt, (index + 1) % 3, coCross(coGetRow(Mt, (index + 2) % 3), coGetRow(Mt, index)));
                coSetRow(MadjTt, (index + 2) % 3, coCross(coGetRow(Mt, index), coGetRow(Mt, (index + 1) % 3)));
                coMat3 M2 = coTranspose(Mt);
    
                Mone = coOneNorm(M2).x;
                Minf = coInfNorm(M2).x;
    
                det = Mt.c0.x * MadjTt.c0.x + Mt.c1.x * MadjTt.c1.x + Mt.c2.x * MadjTt.c2.x;
            }
        }
    
        coFloat MadjTone = coOneNorm(MadjTt).x;
        coFloat MadjTinf = coInfNorm(MadjTt).x;
    
        coFloat gamma = coSquareRoot(coSquareRoot((MadjTone * MadjTinf) / (Mone * Minf)) / coAbs(det));
    
        coFloat g1 = gamma * 0.5f;
        coFloat g2 = 0.5f / (gamma * det);
    
        for (coUint i = 0; i < 3; i++)
        {
            for (coUint j = 0; j < 3; j++)
            {
                Et[j][i] = Mt[j][i];
                Mt[j][i] = g1 * Mt[j][i] + g2 * MadjTt[j][i];
                Et[j][i] -= Mt[j][i];
            }
        }
    
        Eone = coOneNorm(Et).x;
        Mone = coOneNorm(Mt).x;
        Minf = coInfNorm(Mt).x;
    } while (Eone > Mone * tolerance);
    
    // Q = Mt^T 
    return coTranspose(Mt);
}