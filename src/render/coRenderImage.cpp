// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderImage.h"

coRenderImage::coRenderImage()
	: size(0)
	, defaultView(nullptr)
{

}

coRenderImage::~coRenderImage()
{
	coASSERT(!defaultView);
}

coRenderImage::InitConfig::InitConfig()
	: size(0)
	, type(Type::default)
	, arraySize(1)
{

}
