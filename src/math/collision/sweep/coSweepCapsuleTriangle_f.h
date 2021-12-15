// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/coCppExtensions.h>
#include <math/matrix/coMat3.h>
#include "../query/coQueries.h"
class coCapsule;
class coTriangle;
class coVec3;

class Box
{
public:
	/**
	\brief Constructor
	*/
	coFORCE_INLINE Box()
	{
	}

	/**
	\brief Constructor

	\param origin Center of the OBB
	\param extent Extents/radii of the obb.
	\param base rotation to apply to the obb.
	*/
	//! Construct from center, extent and rotation
	coFORCE_INLINE Box(const coVec3& origin, const coVec3& extent, const coMat3& base) : rot(base), center(origin), extents(extent)
	{}

	//! Copy constructor
	coFORCE_INLINE Box(const Box& other) : rot(other.rot), center(other.center), extents(other.extents)
	{}

	/**
	\brief Destructor
	*/
	coFORCE_INLINE ~Box()
	{
	}

	//! Assignment operator
	coFORCE_INLINE const Box& operator=(const Box& other)
	{
		rot = other.rot;
		center = other.center;
		extents = other.extents;
		return *this;
	}

	coMat3	rot;
	coVec3	center;
	coVec3	extents;
};
//static_assert(sizeof(Box) == 60);

//! A padded version of Gu::Box, to safely load its data using SIMD
class BoxPadded : public Box
{
public:
	coFORCE_INLINE BoxPadded() {}
	coFORCE_INLINE ~BoxPadded() {}
	coUint32	padding;
};
//static_assert(sizeof(BoxPadded) == 64);

coBool coSweepCapsuleTriangles_Precise(coUint32 nbTris, const coTriangle* coRESTRICT triangles,
	const coCapsule& capsule,
	const coVec3& unitDir, const coFloat distance,
	const coUint32* coRESTRICT cachedIndex,
	coSweepHit& hit, coVec3& triNormalOut,
	coUint16 hitFlags, coBool doubleSided,
	const BoxPadded* cullBox = nullptr);
