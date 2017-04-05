// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_math/pch.h"
#include "test/unit/coTest.h"
#include "math/transform/coTransform_f.h"

coTEST(coTransform_f, coInverseTransformVector)
{
	const coFloatx3 epsilon = 0.01f;

	// Rotation
	{
		{
			coTransform t;
			t.rotation = coRotation(coFloatx3(0, 0, 1), coFloat_halfPi);
			coFloatx3 r;
			r = coInverseTransformVector(t, coFloatx3(0, 0, 3));
			coEXPECT(coNearEqual(r, coFloatx3(0, 0, 3), epsilon));
			r = coInverseTransformVector(t, coFloatx3(3, 0, 0));
			coEXPECT(coNearEqual(r, coFloatx3(0, -3, 0), epsilon));
			r = coInverseTransformVector(t, coFloatx3(-3, 0, 0));
			coEXPECT(coNearEqual(r, coFloatx3(0, 3, 0), epsilon));
			r = coInverseTransformVector(t, coFloatx3(0, -3, 0));
			coEXPECT(coNearEqual(r, coFloatx3(-3, 0, 0), epsilon));
		}
		
		{
			coTransform t;
			t.rotation = coRotation(coFloatx3(0, 1, 0), coFloat_halfPi);
			coFloatx3 r;
			r = coInverseTransformVector(t, coFloatx3(0, 3, 0));
			coEXPECT(coNearEqual(r, coFloatx3(0, 3, 0), epsilon));
			r = coInverseTransformVector(t, coFloatx3(3, 0, 0));
			coEXPECT(coNearEqual(r, coFloatx3(0, 0, 3), epsilon));
			r = coInverseTransformVector(t, coFloatx3(-3, 0, 0));
			coEXPECT(coNearEqual(r, coFloatx3(0, 0, -3), epsilon));
			r = coInverseTransformVector(t, coFloatx3(0, 0, -3));
			coEXPECT(coNearEqual(r, coFloatx3(3, 0, 0), epsilon));
		}
	}

	// Translation
	{
		coTransform t;
		t.translation = coFloatx3(7, 8, 9);
		coFloatx3 r;
		r = coInverseTransformVector(t, coFloatx3(1, 0, 0));
		coEXPECT(coNearEqual(r, coFloatx3(1, 0, 0), epsilon));
		r = coInverseTransformVector(t, coFloatx3(-1, 0, 0));
		coEXPECT(coNearEqual(r, coFloatx3(-1, 0, 0), epsilon));
		r = coInverseTransformVector(t, coFloatx3(0, -1, 0));
		coEXPECT(coNearEqual(r, coFloatx3(0, -1, 0), epsilon));
	}

	// Scale
	{
		coTransform t;
		t.scale = coFloatx3(7, 8, 9);
		coFloatx3 r;
		r = coInverseTransformVector(t, coFloatx3(1, 0, 0));
		coEXPECT(coNearEqual(r, coFloatx3(1.f/7.f, 0, 0), epsilon));
		r = coInverseTransformVector(t, coFloatx3(-1, 0, 0));
		coEXPECT(coNearEqual(r, coFloatx3(-1.f/7.f, 0, 0), epsilon));
		r = coInverseTransformVector(t, coFloatx3(0, -1, 0));
		coEXPECT(coNearEqual(r, coFloatx3(0, -1.f/8.f, 0), epsilon));
	}

	// Rotation Translation Scale
	{
		coTransform t;
		t.rotation = coRotation(coFloatx3(0, 0, 1), coFloat_halfPi);
		t.translation = coFloatx3(10, 11, 12);
		t.scale = coFloatx3(7, 8, 9);
		coFloatx3 r;
		r = coInverseTransformVector(t, coFloatx3(1, 0, 0));
		coEXPECT(coNearEqual(r, coFloatx3(0, -1.f / 8.f, 0), epsilon));
		r = coInverseTransformVector(t, coFloatx3(-1, 0, 0));
		coEXPECT(coNearEqual(r, coFloatx3(0, 1.f / 8.f, 0), epsilon));
		r = coInverseTransformVector(t, coFloatx3(0, -1, 0));
		coEXPECT(coNearEqual(r, coFloatx3(-1.0 / 7.f, 0, 0), epsilon));
	}
}

coTEST(coTransform_f, operatorMultiply)
{
	{
		const coTransform t = coTransform() * coTransform();
		coEXPECT(coNearEqual(t, coTransform()));
	}

	{
		coTransform rot;
		rot.rotation = coRotation(coFloatx3(0, 0, coFloat_halfPi));
		coEXPECT(coNearEqual(coTransform()*rot, rot));
		coEXPECT(coNearEqual(rot*coTransform(), rot));
	}
}