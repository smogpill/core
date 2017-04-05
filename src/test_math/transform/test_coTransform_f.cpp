// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "test_math/pch.h"
#include "test/unit/coTest.h"
#include "math/transform/coTransform_f.h"

coTEST(coTransform_f, coInverseTransformVector)
{
	// Rotation
	{
		{
			coTransform t;
			t.rotation = coRotation(coFloatx3(0, 0, 1), coFloat_halfPi);
			coFloatx3 r;
			r = coInverseTransformVector(t, coFloatx3(0, 0, 3));
			coEXPECT(coNearEqual(r, coFloatx3(0, 0, 3)));
			r = coInverseTransformVector(t, coFloatx3(3, 0, 0));
			coEXPECT(coNearEqual(r, coFloatx3(0, 3, 0)));
			r = coInverseTransformVector(t, coFloatx3(-3, 0, 0));
			coEXPECT(coNearEqual(r, coFloatx3(0, -3, 0)));
			r = coInverseTransformVector(t, coFloatx3(0, -3, 0));
			coEXPECT(coNearEqual(r, coFloatx3(3, 0, 0)));
		}
		
		{
			coTransform t;
			t.rotation = coRotation(coFloatx3(0, 1, 0), coFloat_halfPi);
			coFloatx3 r;
			r = coInverseTransformVector(t, coFloatx3(0, 3, 0));
			coEXPECT(coNearEqual(r, coFloatx3(0, 3, 0)));
			r = coInverseTransformVector(t, coFloatx3(3, 0, 0));
			coEXPECT(coNearEqual(r, coFloatx3(0, 0, -3)));
			r = coInverseTransformVector(t, coFloatx3(-3, 0, 0));
			coEXPECT(coNearEqual(r, coFloatx3(0, 0, 3)));
			r = coInverseTransformVector(t, coFloatx3(0, 0, -3));
			coEXPECT(coNearEqual(r, coFloatx3(-3, 0, 0)));
		}
	}

	// Translation
	{
		coTransform t;
		t.translation = coFloatx3(7, 8, 9);
		coFloatx3 r;
		r = coInverseTransformVector(t, coFloatx3(1, 0, 0));
		coEXPECT(coNearEqual(r, coFloatx3(1, 0, 0)));
		r = coInverseTransformVector(t, coFloatx3(-1, 0, 0));
		coEXPECT(coNearEqual(r, coFloatx3(-1, 0, 0)));
		r = coInverseTransformVector(t, coFloatx3(0, -1, 0));
		coEXPECT(coNearEqual(r, coFloatx3(0, -1, 0)));
	}

	// Scale
	{
		coTransform t;
		t.scale = coFloatx3(7, 8, 9);
		coFloatx3 r;
		r = coInverseTransformVector(t, coFloatx3(1, 0, 0));
		coEXPECT(coNearEqual(r, coFloatx3(1.f/7.f, 0, 0)));
		r = coInverseTransformVector(t, coFloatx3(-1, 0, 0));
		coEXPECT(coNearEqual(r, coFloatx3(-1.f/7.f, 0, 0)));
		r = coInverseTransformVector(t, coFloatx3(0, -1, 0));
		coEXPECT(coNearEqual(r, coFloatx3(0, -1.f/8.f, 0)));
	}

	// Rotation Translation Scale
	{
		coTransform t;
		t.rotation = coRotation(coFloatx3(0, 0, 1), coFloat_halfPi);
		t.translation = coFloatx3(10, 11, 12);
		t.scale = coFloatx3(7, 8, 9);
		coFloatx3 r;
		r = coInverseTransformVector(t, coFloatx3(1, 0, 0));
		coEXPECT(coNearEqual(r, coFloatx3(0, 1.f / 8.f, 0)));
		r = coInverseTransformVector(t, coFloatx3(-1, 0, 0));
		coEXPECT(coNearEqual(r, coFloatx3(0, -1.f / 8.f, 0)));
		r = coInverseTransformVector(t, coFloatx3(0, -1, 0));
		coEXPECT(coNearEqual(r, coFloatx3(1.0 / 7.f, 0, 0)));
	}
}
