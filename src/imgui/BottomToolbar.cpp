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
		GetWorkspace().GetOnLoadArchiveFailed() += std::bind(&BottomToolbar::OnLoadArchiveFailed, this, std::placeholders::_1, std::placeholders::_2);
		GetWorkspace().GetOnLoadArchiveSuccess() += std::bind(&BottomToolbar::OnLoadArchiveSuccess, this, std::placeholders::_1);
		GetWorkspace().GetOnLoadArchiveProgressed() += std::bind(&BottomToolbar::OnLoadArchiveProgressed, this, std::placeholders::_1);
	}

	//---------------------------------------------------------------------
	void BottomToolbar::Render()
	{
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
			if (m_fPercentage == 0 && m_sMessage.empty())
			{

			}
			else if (m_sMessage.empty() && m_fPercentage > 0.0f)
			{
				float progress = 0.65f;

				const float barWidth = 300.0f;
				const float barHeight = 30.0f;

				ImVec2 contentSize = ImGui::GetContentRegionAvail();

				ImGui::SetCursorPos(
					ImVec2(
						padding.x,
						(contentSize.y - barHeight) * 0.5f
					)
				);

				ImGui::ProgressBar(progress, ImVec2(barWidth, barHeight));
			}
			else
			{
				ImDrawList* drawList = ImGui::GetWindowDrawList();

				ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
				float iconSize = 32;
				float iconY = cursorScreenPos.y + (m_fSize - iconSize) * 0.5f;

				ImGui::PushFont(GetDefaultFont());
				// Icon
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

					std::string text = m_sMessage;

					drawList->AddText(
						ImVec2(iconMin.x + iconSize + padding.x, iconY +
							(iconSize - ImGui::CalcTextSize(text.c_str()).y) * 0.5f),
						ImGui::GetColorU32(ImGuiCol_Text),
						text.c_str()
					);
				}
				ImGui::PopFont();
			}
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
	void BottomToolbar::OnLoadArchiveSuccess(const std::string& a_sName)
	{
		m_sMessage = "Archive Loaded Successfully.";
		m_sIcon = "../icons/icon_checkmark.svg";
		m_fPercentage = 0.0f;
	}

	//---------------------------------------------------------------------
	void BottomToolbar::OnLoadArchiveFailed(const std::string& a_sName, const std::string& a_sReason)
	{
		m_sMessage = "Failed to load archive: " + a_sReason + ".";
		m_sIcon = "../icons/icon_error.svg";
		m_fPercentage = 0.0f;
	}

	//---------------------------------------------------------------------
	void BottomToolbar::OnLoadArchiveProgressed(float a_fProgress)
	{
		m_fPercentage = a_fProgress;
	}
}