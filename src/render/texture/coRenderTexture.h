// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/result/coResult.h>
class coImage;

class coRenderTexture
{
public:
	coRenderTexture();
	~coRenderTexture();

	coResult SetContent(const coImage& image);
	void Bind(coUint unit);
	static void Unbind(coUint unit);
	GLuint GetGLID() const { return id; }

	static inline const coUint8 maxNbUnits = 16; // No need more

private:
	GLuint id = 0;
};
