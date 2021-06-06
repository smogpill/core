// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "imgui/pch.h"
#include "imgui/coImgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"

coImgui::~coImgui()
{
	ImGui_ImplOpenGL3_Shutdown();
}

coResult coImgui::Init()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	return ImGui_ImplOpenGL3_Init();
}

void coImgui::DrawDemo()
{
	ImGui::ShowDemoWindow(&demo);
}

void coImgui::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
}

void coImgui::End()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
