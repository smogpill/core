// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

/**
*	PT: computes "alignment value" used to select the "best" triangle in case of identical impact distances (for sweeps).
*	This simply computes how much a triangle is aligned with a given sweep direction.
*	Captured in a function to make sure it is always computed correctly, i.e. working for double-sided triangles.
*
*	\param		triNormal	[in] triangle's normal
*	\param		unitDir		[in] sweep direction (normalized)
*	\return		alignment value in [-1.0f, 0.0f]. -1.0f for fully aligned, 0.0f for fully orthogonal.
*/
coFORCE_INLINE coFloat coComputeAlignmentValue(const coVec3& triNormal, const coVec3& unitDir)
{
	// PT: initial dot product gives the angle between the two, with "best" triangles getting a +1 or -1 score
	// depending on their winding. We take the absolute value to ignore the impact of winding. We negate the result
	// to make the function compatible with the initial code, which assumed single-sided triangles and expected -1
	// for best triangles.
	return -coAbs(coDot(triNormal, unitDir)).x;
}

/**
	*	PT: sweeps: determines if a newly touched triangle is "better" than best one so far.
	*	In this context "better" means either clearly smaller impact distance, or a similar impact
	*	distance but a normal more aligned with the sweep direction.
	*
	*	\param		triImpactDistance	[in] new triangle's impact distance
	*	\param		triAlignmentValue	[in] new triangle's alignment value (as computed by computeAlignmentValue)
	*	\param		bestImpactDistance	[in] current best triangle's impact distance
	*	\param		bestAlignmentValue	[in] current best triangle's alignment value (as computed by computeAlignmentValue)
	*   \param		maxDistance			[in] maximum distance of the query, hit cannot be longer than this maxDistance
	*	\param		distEpsilon			[in] tris have "similar" impact distances if the difference is smaller than 2*distEpsilon
	*	\return		true if new triangle is better
	*/
coFORCE_INLINE coBool coKeepTriangle(coFloat triImpactDistance, coFloat triAlignmentValue,
	coFloat bestImpactDistance, coFloat bestAlignmentValue, coFloat maxDistance,
	coFloat distEpsilon)
{
	// Reject triangle if further than the maxDistance
	if (triImpactDistance > maxDistance)
		return false;

	// PT: make it a relative epsilon to make sure it still works with large distances
	distEpsilon *= coMax(1.0f, coMax(triImpactDistance, bestImpactDistance));

	// If new distance is more than epsilon closer than old distance
	if (triImpactDistance < bestImpactDistance - distEpsilon)
		return true;

	// If new distance is no more than epsilon farther than oldDistance and "face is more opposing than previous"
	if (triImpactDistance < bestImpactDistance + distEpsilon && triAlignmentValue < bestAlignmentValue)
		return true;

	// If alignment value is the same, but the new triangle is closer than the best distance
	if (triAlignmentValue == bestAlignmentValue && triImpactDistance < bestImpactDistance)
		return true;

	// If initial overlap happens, keep the triangle
	if (triImpactDistance == 0.0f)
		return true;

	return false;
}