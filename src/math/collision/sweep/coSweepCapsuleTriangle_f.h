// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/coCppExtensions.h>
class coCapsule;
class coTriangle;
class coVec3;

struct coHitFlag
{
	enum Enum
	{
		ePOSITION = (1 << 0),					//!< "position" member of #PxQueryHit is valid
		eNORMAL = (1 << 1),						//!< "normal" member of #PxQueryHit is valid
		eUV = (1 << 3),							//!< "u" and "v" barycentric coordinates of #PxQueryHit are valid. Not applicable to sweep queries.
		eASSUME_NO_INITIAL_OVERLAP = (1 << 4),	//!< Performance hint flag for sweeps when it is known upfront there's no initial overlap.
												//!< NOTE: using this flag may cause undefined results if shapes are initially overlapping.
		eMESH_MULTIPLE = (1 << 5),				//!< Report all hits for meshes rather than just the first. Not applicable to sweep queries.
		eMESH_ANY = (1 << 6),					//!< Report any first hit for meshes. If neither eMESH_MULTIPLE nor eMESH_ANY is specified,
												//!< a single closest hit will be reported for meshes.
		eMESH_BOTH_SIDES = (1 << 7),			//!< Report hits with back faces of mesh triangles. Also report hits for raycast
												//!< originating on mesh surface and facing away from the surface normal. Not applicable to sweep queries.
												//!< Please refer to the user guide for heightfield-specific differences.
		ePRECISE_SWEEP = (1 << 8),				//!< Use more accurate but slower narrow phase sweep tests.
												//!< May provide better compatibility with PhysX 3.2 sweep behavior.
		eMTD = (1 << 9),						//!< Report the minimum translation depth, normal and contact point.
		eFACE_INDEX = (1 << 10),				//!< "face index" member of #PxQueryHit is valid

		eDEFAULT = ePOSITION | eNORMAL | eFACE_INDEX,

		/** \brief Only this subset of flags can be modified by pre-filter. Other modifications will be discarded. */
		eMODIFIABLE_FLAGS = eMESH_MULTIPLE | eMESH_BOTH_SIDES | eASSUME_NO_INITIAL_OVERLAP | ePRECISE_SWEEP
	};
};

struct coSweepHit : public PxLocationHit
{
	PX_INLINE			PxSweepHit() {}

	coUint32			padTo16Bytes;
};

coBool coSweepCapsuleTriangles_Precise(coUint32 nbTris, const coTriangle* coRESTRICT triangles,
	const coCapsule& capsule,
	const coVec3& unitDir, const coFloat distance,
	const coUint32* coRESTRICT cachedIndex,
	PxSweepHit& hit, coVec3& triNormalOut,
	coUint16 hitFlags, coBool doubleSided,
	const BoxPadded* cullBox = nullptr);
