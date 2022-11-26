// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

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
		eWAS_INITIAL_OVERLAP = (1 << 11),

		eDEFAULT = ePOSITION | eNORMAL | eFACE_INDEX,

		/** \brief Only this subset of flags can be modified by pre-filter. Other modifications will be discarded. */
		eMODIFIABLE_FLAGS = eMESH_MULTIPLE | eMESH_BOTH_SIDES | eASSUME_NO_INITIAL_OVERLAP | ePRECISE_SWEEP
	};
};

typedef coUint16 coHitFlags;

/**
\brief Combines a shape pointer and the actor the shape belongs to into one memory location.

Serves as a base class for PxQueryHit.

@see PxQueryHit
*/
class coActorShape
{
public:
	//coActorShape() : actor(nullptr), shape(nullptr) {}
	//coActorShape(PxRigidActor* a, PxShape* s) : actor(a), shape(s) {}

	//PxRigidActor* actor;
	//PxShape* shape;
};


/**
\brief Scene query hit information.
*/
class coQueryHit : public coActorShape
{
public:
	coQueryHit() : faceIndex(0xFFFFffff) {}

	/**
	Face index of touched triangle, for triangle meshes, convex meshes and height fields.

	\note This index will default to 0xFFFFffff value for overlap queries.
	\note Please refer to the user guide for more details for sweep queries.
	\note This index is remapped by mesh cooking. Use #PxTriangleMesh::getTrianglesRemap() to convert to original mesh index.
	\note For convex meshes use #PxConvexMesh::getPolygonData() to retrieve touched polygon data.
	*/
	coUint32 faceIndex;
};

/**
\brief Scene query hit information for raycasts and sweeps returning hit position and normal information.

::PxHitFlag flags can be passed to scene query functions, as an optimization, to cause the SDK to
only generate specific members of this structure.
*/
class coLocationHit : public coQueryHit
{
public:
	coLocationHit() : flags(0), position(coVec3(0)), normal(coVec3(0)), distance(1e30f) {}

	/**
	\note For raycast hits: true for shapes overlapping with raycast origin.
	\note For sweep hits: true for shapes overlapping at zero sweep distance.

	@see PxRaycastHit PxSweepHit
	*/
	bool		hadInitialOverlap() const { return (distance <= 0.0f); }

	// the following fields are set in accordance with the #PxHitFlags
	coHitFlags			flags;		//!< Hit flags specifying which members contain valid values.
	coVec3				position;	//!< World-space hit position (flag: #PxHitFlag::ePOSITION)
	coVec3				normal;		//!< World-space hit normal (flag: #PxHitFlag::eNORMAL)

	/**
	\brief	Distance to hit.
	\note	If the eMTD flag is used, distance will be a negative value if shapes are overlapping indicating the penetration depth.
	\note	Otherwise, this value will be >= 0 */
	coFloat distance;
};

class coSweepHit : public coLocationHit
{
public:
	coUint32 padTo16Bytes;
};