#include "PreviewWindow.h"

#include <imgui/imgui.h>

#include "dx11/SVGTextureCache.h"

#include "imgui/ImGuiSetup.h"
#include "imgui/Helpers.h"

namespace humongousexplorer::imgui
{
	const float PreviewWindow::s_aPresets[s_iPresetCount] = { 0.25f, 0.50f, 0.75f, 1.0f, 1.5f, 2.0f };
	const char* PreviewWindow::s_aPresetLabels[s_iPresetCount] = { "25%", "50%", "75%", "100%", "150%", "200%" };

	//---------------------------------------------------------------------
	PreviewWindow::PreviewWindow() : HEBaseWindow(ImGuiWindowFlags_NoCollapse, "PREVIEW", "PreviewWindow")
	{
	}

	//---------------------------------------------------------------------
	bool PreviewWindow::Initialize()
	{
		return HEBaseWindow::Initialize();
	}

	//---------------------------------------------------------------------
	void PreviewWindow::RenderCheckerboard(const ImVec2& a_vMin, const ImVec2& a_vMax)
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		float squareSize = 25.0f;

		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;
		ImU32 colorA = ImGui::ColorConvertFloat4ToU32(colors[ImGuiCol_TableRowBg]);
		ImU32 colorB = ImGui::ColorConvertFloat4ToU32(colors[ImGuiCol_TableRowBgAlt]);

		int startX = static_cast<int>(a_vMin.x);
		int startY = static_cast<int>(a_vMin.y);
		int endX = static_cast<int>(a_vMax.x + 0.5f);
		int endY = static_cast<int>(a_vMax.y + 0.5f);

		for (int y = startY; y < endY; y += static_cast<int>(squareSize))
		{
			for (int x = startX; x < endX; x += static_cast<int>(squareSize))
			{
				int checkerX = (x - startX) / static_cast<int>(squareSize);
				int checkerY = (y - startY) / static_cast<int>(squareSize);
				ImU32 color = ((checkerX + checkerY) % 2 == 0) ? colorA : colorB;

				float rectX = static_cast<float>(x);
				float rectY = static_cast<float>(y);
				float rectW = (std::min)(squareSize, static_cast<float>(endX - x));
				float rectH = (std::min)(squareSize, static_cast<float>(endY - y));

				drawList->AddRectFilled(
					ImVec2(rectX, rectY),
					ImVec2(rectX + rectW, rectY + rectH),
					color
				);
			}
		}

		ImU32 borderColor = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_Border));
		ImGui::GetWindowDrawList()->AddRect(
			a_vMin,
			a_vMax,
			borderColor,
			0.0f,
			0
		);
	}

	//---------------------------------------------------------------------
	void PreviewWindow::RenderControlsBar()
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);

		// TODO: Fill metadata.
		ImGui::Text("640 x 480");
		ImGui::SameLine();
		ImGui::Text("|");
		ImGui::SameLine();
		ImGui::Text("8-bit Indexed (256 colors)");

		// Zoom controls in the center
		float totalControlsWidth = 28.0f + 100.0f + 28.0f;
		float availWidth = ImGui::GetContentRegionAvail().x;
		float offsetX = (availWidth - totalControlsWidth) * 0.5f;
		if (offsetX > 0.0f)
		{
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
		}
		else
		{
			ImGui::SameLine();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 2.0f));

		if (ImGui::Button(FormatId("-", BUTTON_ID, "PreviewWindow", "ZoomOut").c_str(), ImVec2(24.0f, 0.0f)))
		{
			for (int i = s_iPresetCount - 1; i >= 0; i--)
			{
				if (s_aPresets[i] < m_fZoom - 0.001f)
				{
					m_fZoom = s_aPresets[i];
					break;
				}
			}
		}
		ImGui::SameLine();

		// Zoom combo — shows current zoom as percentage
		char zoomLabel[16];
		snprintf(zoomLabel, sizeof(zoomLabel), "%.0f%%", m_fZoom * 100.0f);
		ImGui::PushItemWidth(90.0f);
		if (ImGui::BeginCombo(FormatId("", DROPDOWN_ID, "PreviewWindow", "Zoom").c_str(), zoomLabel))
		{
			for (int i = 0; i < s_iPresetCount; i++)
			{
				bool isSelected = (fabsf(m_fZoom - s_aPresets[i]) < 0.001f);
				if (ImGui::Selectable(s_aPresetLabels[i], isSelected))
					m_fZoom = s_aPresets[i];
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();

		if (ImGui::Button(FormatId("+", BUTTON_ID, "PreviewWindow", "ZoomIn").c_str(), ImVec2(24.0f, 0.0f)))
		{
			for (int i = 0; i < s_iPresetCount; i++)
			{
				if (s_aPresets[i] > m_fZoom + 0.001f)
				{
					m_fZoom = s_aPresets[i];
					break;
				}
			}
		}

		ImGui::SameLine();
		if (ImGui::Button(FormatId("Reset", BUTTON_ID, "Reset").c_str()))
		{
			m_fZoom = 1.0f;
			m_vPan = ImVec2(0.0f, 0.0f);
		}

		ImGui::PopStyleVar();

		ImGui::SameLine();
		float rightOffset = ImGui::GetContentRegionAvail().x - 28.0f;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + rightOffset);

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));

		bool prevCheckerboard = m_bShowCheckerboard;
		if (m_bShowCheckerboard)
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));

		if (ImGui::ImageButton(FormatId("", BUTTON_ID, "PreviewWindow", "Checkerboard").c_str(),
			(ImTextureID)dx11::SVGTextureCache::Get("../icons/icon_transparent.svg"),
			ImVec2(ImGui::GetFontSize() + 2.0f, ImGui::GetFontSize() + 2.0f)))
		{
			m_bShowCheckerboard = !m_bShowCheckerboard;
		}
		if (prevCheckerboard)
			ImGui::PopStyleColor();

		ImGui::PopStyleVar();
	}

	//---------------------------------------------------------------------
	void PreviewWindow::Update()
	{
		ImVec2 avail = ImGui::GetContentRegionAvail();
		float controlsBarHeight = ImGui::GetFrameHeight() + 16.0f;
		ImVec2 previewArea(avail.x, avail.y - controlsBarHeight);

		if (previewArea.x <= 0 || previewArea.y <= 0)
			return;

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 areaMin = ImGui::GetCursorScreenPos();
		ImVec2 areaMax(areaMin.x + previewArea.x, areaMin.y + previewArea.y);

		// Checkerboard background
		if (m_bShowCheckerboard)
		{
			RenderCheckerboard(areaMin, areaMax);
		}
		else
		{
			drawList->AddRectFilled(areaMin, areaMax, IM_COL32(30, 30, 30, 255));
		}

		// Mouse wheel zoom when hovering the preview area
		if (ImGui::IsMouseHoveringRect(areaMin, areaMax))
		{
			float wheel = ImGui::GetIO().MouseWheel * 0.5f;
			if (wheel != 0.0f)
			{
				m_fZoom += wheel * 0.1f;
				if (m_fZoom < 0.05f) m_fZoom = 0.05f;
				if (m_fZoom > 16.0f) m_fZoom = 16.0f;
			}
		}

		// Mouse drag panning when hovering the preview area
		if (ImGui::IsMouseHoveringRect(areaMin, areaMax) && ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
		{
			ImVec2 delta = ImGui::GetIO().MouseDelta;
			m_vPan.x += delta.x;
			m_vPan.y += delta.y;
		}

		float imgW = 320.0f * m_fZoom;
		float imgH = 200.0f * m_fZoom;
		float centerX = areaMin.x + previewArea.x * 0.5f + m_vPan.x;
		float centerY = areaMin.y + previewArea.y * 0.5f + m_vPan.y;
		ImVec2 imgMin(centerX - imgW * 0.5f, centerY - imgH * 0.5f);
		ImVec2 imgMax(centerX + imgW * 0.5f, centerY + imgH * 0.5f);

		// Clip to preview area
		drawList->PushClipRect(areaMin, areaMax, true);
		drawList->AddRectFilled(imgMin, imgMax, IM_COL32(80, 60, 135, 255));
		drawList->AddRect(imgMin, imgMax, IM_COL32(160, 130, 255, 255));
		drawList->PopClipRect();

		// Reserve space for the preview area
		ImGui::Dummy(previewArea);

		// Controls bar at the bottom
		RenderControlsBar();
	}
}
