// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coRenderSampler
{
public:
	enum class WrapMode
	{
		REPEAT,
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER
	};
	enum class FilterMode
	{
		NEAREST,
		LINEAR,
		LINEAR_MIPMAP_LINEAR
	};
	coRenderSampler();
	~coRenderSampler();

	void SetWrapMode(WrapMode);
	void SetFilterMode(FilterMode mode);
	void SetMaxAnisotropy(coFloat anisotropy);
	GLuint GetGLID() const { return glID; }

private:
	GLuint glID = 0;
};
