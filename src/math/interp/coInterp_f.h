// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

template <typename T>
T coSmoothInterp(T& velocityInOut, const T& from, const T& to, const T& smoothTime, const coFloat dt)
{
	// The implementation is based on the Game Programming Gems 4 article, 
	// but it can easily be rewritten starting from the maths behind critical damped systems.
	// The SmoothTime is different from the one described in the Game Programming Gems article though, 
	// SmoothTime = (articleLagTime / 4). It better reflects the user's intent.
	const T Omega4 = T(8.) / (smoothTime + T(0.0001)); // small value to avoid division by 0
	const T X = Omega4 * dt;
	const T X2 = X * X;
	const T Denom = (T(1.) + X + T(0.48) * X2 + T(0.235) * X2 * X);
	const T Exp = T(1.) / Denom;
	const T Change = from - to;
	const T Temp = (velocityInOut + (Change * Omega4)) * T(dt);
	velocityInOut = (velocityInOut - (Temp * Omega4)) * Exp;
	return to + (Change + Temp) * Exp;
}
