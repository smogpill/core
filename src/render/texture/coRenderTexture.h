// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/result/coResult.h>
#include <math/vector/coUint32x2.h>
class coImage;

class coRenderTexture
{
public:
	coRenderTexture();
	~coRenderTexture();

	coResult SetContent(const coImage& image);
	void SetNearestFiltering(coBool b) { nearestFiltering = b; }
	void SetMipMaps(coBool b) { mipMaps = b; }
	coUint32x2 GetSize() const { return size; }
	GLuint GetGLID() const { return id; }

	static inline const coUint8 maxNbUnits = 32; // No need more

private:
	GLuint id = 0;
	coUint32x2 size = 0;
	coBool nearestFiltering : 1;
	coBool mipMaps : 1;
};
