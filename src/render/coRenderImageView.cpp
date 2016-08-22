// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderImageView.h"
#include "lang/result/coResult_f.h"

coRenderImageView::coRenderImageView()
	: image(nullptr)
{

}

coRenderImageView::InitConfig::InitConfig()
	: image(nullptr)
	, type(Type::default_)
{

}

coResult coRenderImageView::OnInit(const coObject::InitConfig& _config)
{
	coTRY(Super::OnInit(_config), nullptr);
	const InitConfig& config = static_cast<const InitConfig&>(_config);
	image = config.image;
	coTRY(image, nullptr);
	return true;
}
