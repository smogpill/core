// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "imgui/pch.h"
#include "imgui/coImgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "app/window/coWindow.h"

coImgui::~coImgui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext(context);
}

coResult coImgui::Init()
{
	context = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.DisplayFramebufferScale = ImVec2(1, 1);
	ImGui::StyleColorsDark();
	return ImGui_ImplOpenGL3_Init();
}

void coImgui::DrawDemo()
{
	ImGui::ShowDemoWindow(&demo);
}

void coImgui::Begin()
{
	ImGuiIO& io = ImGui::GetIO();
	if (window)
	{
		const coInt32x2& size = window->GetClientSize();
		io.DisplaySize = ImVec2(coFloat(size.x), coFloat(size.y));
	}
	else
	{
		io.DisplaySize = ImVec2(64, 64);
	}
	io.DeltaTime = 1.0f / 60.0f;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
}

void coImgui::End()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
