// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "gui_imgui/pch.h"
#include "gui_imgui/immediate/coDearImGuiImpl.h"
#include "gui/immediate/coImGui.h"
#include "gui/immediate/coImGuiImpl.h"
#include "lang/result/coResult_f.h"

coImGui::coImGui()
	: coImGui(*(new coDearImGuiImpl()))
{

}

coImGui::~coImGui()
{
	ImGui::Shutdown();
	delete impl;
}

coResult coImGui::OnInit()
{
	coTRY(Super::OnInit(), nullptr);

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(800, 600);
	io.DisplayFramebufferScale = ImVec2(1, 1);

	return true;
}

void coImGui::AddText(const coChar*_s)
{
	ImGui::Text(_s);
}

void coImGui::AddSeparator()
{
	ImGui::Separator();
}

void coImGui::BeginWindow(const coChar* _name)
{
	ImGui::Begin(_name);
}

void coImGui::EndWindow()
{
	ImGui::End();
}

coResult coImGui::BeginFrame()
{
	ImGuiIO& io_di = ImGui::GetIO();
	io_di.DeltaTime = 1.0f / 60.0f;
	ImGui::NewFrame();
	return true;
}

coResult coImGui::EndFrame()
{
	return true;
}

coResult Render()
{
	ImGui::Render();
	//ImDrawData* data_di = ImGui::GetDrawData();
	return true;
}
