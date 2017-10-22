// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "gui_imgui/pch.h"
#include "gui_imgui/immediate/coDearImGuiImpl.h"
#include "gui/immediate/coImGui.h"
#include "gui/immediate/coImGuiImpl.h"
#include "lang/result/coResult_f.h"
#include "math/scalar/coUint32_f.h"
#include "render/coRenderCommandBuffer.h"
#include "render/coRenderFactory.h"
#include "io/file/coFile_f.h"

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

	coDearImGuiImpl* p = static_cast<coDearImGuiImpl*>(impl);
	coTRY(p->device, nullptr);

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(800, 600);
	io.DisplayFramebufferScale = ImVec2(1, 1);

	coTRY(p->InitMaterial(), nullptr);

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

coResult coImGui::Render(coRenderCommandBuffer& _commandBuffer)
{
	ImGui::Render();
	ImDrawData* data = ImGui::GetDrawData();

	coUint vertexOffset = 0;
	coUint indexOffset = 0;
	for (coUint i = 0; i < coUint(data->CmdListsCount); ++i)
	{
		const ImDrawList* cmdList = data->CmdLists[i];
		const auto& cmdBuffer = cmdList->CmdBuffer;
		for (coUint j = 0; j < coUint(cmdBuffer.Size); ++j)
		{
			const ImDrawCmd& cmd = cmdBuffer[j];
			if (cmd.UserCallback)
			{
				cmd.UserCallback(cmdList, &cmd);
			}
			else
			{
				const coUint x = coUint(coMax(0, coInt(cmd.ClipRect.x)));
				const coUint y = coUint(coMax(0, coInt(cmd.ClipRect.y)));
				const coUint width = coUint(cmd.ClipRect.z - cmd.ClipRect.x);
				const coUint height = coUint(cmd.ClipRect.w - cmd.ClipRect.y + 1); // FIXME: Why +1 here?
				_commandBuffer.PushSetScissor(x, y, width, height);
				_commandBuffer.PushDraw(cmd.ElemCount, vertexOffset, indexOffset);
			}
			indexOffset += cmd.ElemCount;
		}
		vertexOffset += cmdList->VtxBuffer.Size;
	}

	return true;
}
