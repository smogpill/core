// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
class coConstString;

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
	void SetDebugLabel(const coConstString& label);
	GLuint GetGLID() const { return id; }

private:
	GLuint id = 0;
};
