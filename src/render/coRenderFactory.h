// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coRenderContext;
class coSwapChain;
class coSurface;
class coRenderSemaphore;

coRenderContext* coCreateRenderer();
coSwapChain* coCreateSwapChain();
coSurface* coCreateSurface();
coRenderSemaphore* coCreateRenderSemaphore();
