// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_math/pch.h"
#include "test/unit/coTest.h"
#include "math/shape/coPlane_f.h"

coTEST(coPlane, coIsValid)
{
	coEXPECT(!coIsValid(coPlane(coFloatx4(0, 0, 0, 0))));
	coEXPECT(!coIsValid(coPlane(coFloatx4(0, 0, 0, 1))));
	coEXPECT(coIsValid(coPlane(coFloatx4(1, 0, 0, 0))));
	coEXPECT(coIsValid(coPlane(coFloatx4(0, 1, 0, 0))));
	coEXPECT(coIsValid(coPlane(coFloatx4(0, 0, 1, 0))));
	coEXPECT(!coIsValid(coPlane(coFloatx4(1, 2, 3, 0))));
	coEXPECT(coIsValid(coPlane(coFloatx4(1, 0, 0, 1))));
}

coTEST(coPlane, coSetFromNormalAndSignedDistance)
{
	coPlane p;
	coSetFromNormalAndSignedDistance(p, coFloatx3(1, 0, 0), coFloatx3(4));
	coEXPECT(p.normalAndDistance == coFloatx4(1, 0, 0, -4));
	coSetFromNormalAndSignedDistance(p, coFloatx3(0, 1, 0), coFloatx3(4));
	coEXPECT(p.normalAndDistance == coFloatx4(0, 1, 0, -4));
	coSetFromNormalAndSignedDistance(p, coFloatx3(0, 0, 1), coFloatx3(4));
	coEXPECT(p.normalAndDistance == coFloatx4(0, 0, 1, -4));
}

coTEST(coPlane, coSetFromNormalAndPoint)
{
	coPlane p;
	coSetFromNormalAndPoint(p, coFloatx3(1, 0, 0), coFloatx3(7, 8, 9));
	coEXPECT(p.normalAndDistance == coFloatx4(1, 0, 0, -7));
	coSetFromNormalAndPoint(p, coFloatx3(0, 1, 0), coFloatx3(7, 8, 9));
	coEXPECT(p.normalAndDistance == coFloatx4(0, 1, 0, -8));
	coSetFromNormalAndPoint(p, coFloatx3(0, 0, 1), coFloatx3(7, 8, 9));
	coEXPECT(p.normalAndDistance == coFloatx4(0, 0, 1, -9));
}

coTEST(coPlane, coDistanceToPoint)
{
	coPlane p;
	coFloatx3 d;

	coSetFromNormalAndSignedDistance(p, coFloatx3(1, 0, 0), coFloatx3(3));
	d = coDistanceToPoint(p, coFloatx3(1, 2, 3));
	coEXPECT(coAreXYZEqual(d));
	coEXPECT(d == coFloatx3(-2));

	coSetFromNormalAndSignedDistance(p, coFloatx3(0, 1, 0), coFloatx3(3));
	d = coDistanceToPoint(p, coFloatx3(1, 2, 3));
	coEXPECT(coAreXYZEqual(d));
	coEXPECT(d == coFloatx3(-1));

	coSetFromNormalAndSignedDistance(p, coFloatx3(0, 0, 1), coFloatx3(3));
	d = coDistanceToPoint(p, coFloatx3(1, 2, 3));
	coEXPECT(coAreXYZEqual(d));
	coEXPECT(d == coFloatx3(0));
}

coTEST(coPlane, coRayPlaneIntersection)
{
	coPlane p;
	coSetFromNormalAndPoint(p, coFloatx3(1, 0, 0), coFloatx3(2, 0, 0));
	coFloatx3 t;
	coEXPECT(coRayPlaneIntersection(p, coFloatx3(3, 2, 1), coFloatx3(-1, 0, 0), t));
	coEXPECT(coAreXYZEqual(t));
	coEXPECT(t == coFloatx3(1));
	coEXPECT(coRayPlaneIntersection(p, coFloatx3(2, 0, 0), coFloatx3(1, 0, 0), t));
	coEXPECT(coAreXYZEqual(t));
	coEXPECT(coNearEqual0(t));
	coEXPECT(!coRayPlaneIntersection(p, coFloatx3(2, 0, 0), coFloatx3(0, 1, 0), t));
	coEXPECT(coAreXYZEqual(t));
	coEXPECT(coNearEqual0(t));
	coEXPECT(!coRayPlaneIntersection(p, coFloatx3(2, 1, 0), coFloatx3(0, 1, 0), t));
	coEXPECT(coAreXYZEqual(t));
	coEXPECT(coNearEqual0(t));
}
