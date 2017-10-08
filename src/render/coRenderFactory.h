// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coRenderer;
class coRenderContext;
class coSwapChain;
class coSurface;
class coRenderSemaphore;
class coRenderPipeline;
class coRenderPipelineLayout;
class coShader;
class coRenderMesh;
class coRenderSampler;

coRenderer* coCreateRenderer();
coRenderContext* coCreateRenderContext();
coSwapChain* coCreateSwapChain();
coSurface* coCreateSurface();
coRenderSemaphore* coCreateRenderSemaphore();
coRenderPipeline* coCreateRenderPipeline();
coRenderPipelineLayout* coCreateRenderPipelineLayout();
coShader* coCreateShader();
coRenderMesh* coCreateRenderMesh();
coRenderSampler* coCreateRenderSampler();
