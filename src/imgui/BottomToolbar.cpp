#include "BottomToolbar.h"

#include <imgui.h>

#include "dx11/SVGTextureCache.h"
#include "imgui/ImGuiSetup.h"
#include "editor/Workspace.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	void BottomToolbar::Initialize()
	{
		if (m_bInitialized)
		{
			return;
		}

		GetWorkspace().GetStatusMessageUpdated() += std::bind(&BottomToolbar::OnStatusMessageUpdated, this, std::placeholders::_1, std::placeholders::_2);

		m_sMessage = "Drop an archive to begin";
		m_sIcon = "../icons/icon_drop_file.svg";

		m_bInitialized = true;
	}

	//---------------------------------------------------------------------
	void BottomToolbar::Render()
	{
		const auto& workspace = GetWorkspace();

		ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(
			ImVec2(viewport->Pos.x, viewport->Pos.y + viewport->Size.y - m_fSize)
		);
		ImGui::SetNextWindowSize(
			ImVec2(viewport->Size.x, m_fSize)
		);

		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoDocking;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImVec2 padding = ImGui::GetStyle().WindowPadding;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		if (ImGui::Begin("BottomMenuBar", nullptr, flags))
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
			float iconSize = 32;
			float iconY = cursorScreenPos.y + (m_fSize - iconSize) * 0.5f;

			ImGui::PushFont(GetDefaultFont());
			ID3D11ShaderResourceView* tex = dx11::SVGTextureCache::Get(m_sIcon);
			if (tex)
			{
				int nativeW = dx11::SVGTextureCache::GetWidth(m_sIcon);
				int nativeH = dx11::SVGTextureCache::GetHeight(m_sIcon);
				float scale = iconSize / static_cast<float>((nativeW > nativeH) ? nativeW : nativeH);
				float drawW = nativeW * scale;
				float drawH = nativeH * scale;

				ImVec2 iconMin = ImVec2(cursorScreenPos.x + padding.x, iconY);
				ImVec2 iconMax = ImVec2(iconMin.x + drawW, iconY + drawH);
				drawList->AddImage(
					static_cast<ImTextureID>(reinterpret_cast<intptr_t>(tex)),
					iconMin,
					iconMax,
					ImVec2(0, 0), ImVec2(1, 1),
					IM_COL32(255, 255, 255, 255));

				drawList->AddText(
					ImVec2(iconMin.x + iconSize + padding.x, iconY +
						(iconSize - ImGui::CalcTextSize(m_sMessage.c_str()).y) * 0.5f),
					ImGui::GetColorU32(ImGuiCol_Text),
					m_sMessage.c_str()
				);
			}
			ImGui::PopFont();
		}
		ImGui::End();

		ImGui::PopStyleVar(2);
	}

	//---------------------------------------------------------------------
	float BottomToolbar::GetSize() const
	{
		return m_fSize;
	}

	//---------------------------------------------------------------------
	void BottomToolbar::OnStatusMessageUpdated(bool a_bSuccess, const std::string& a_sMessage)
	{
		m_sMessage = a_sMessage;
		if (a_bSuccess)
		{
			m_sIcon = "../icons/icon_checkmark.svg";
		}
		else
		{
			m_sIcon = "../icons/icon_error.svg";
		}
	}
}
