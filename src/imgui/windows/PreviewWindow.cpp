#include "PreviewWindow.h"

#include <cmath>
#include <imgui/imgui.h>
#include <imgui/implot.h>
#include <imgui/icon.h>
#include <imgui/Helpers.h>
#include <algorithm>

#include "dx11/SVGTextureCache.h"

#include "imgui/ImGuiSetup.h"
#include "editor/Workspace.h"
#include "resources/Resource.h"
#include "core/Data.h"

namespace humongousexplorer::imgui
{
	const float PreviewWindow::s_aPresets[s_iPresetCount] = { 0.25f, 0.50f, 0.75f, 1.0f, 1.5f, 2.0f };
	const char* PreviewWindow::s_aPresetLabels[s_iPresetCount] = { "25%", "50%", "75%", "100%", "150%", "200%" };

	//---------------------------------------------------------------------
	PreviewWindow::PreviewWindow() : HEBaseWindow(ImGuiWindowFlags_NoCollapse, "PREVIEW", "PreviewWindow")
	{}

	//---------------------------------------------------------------------
	bool PreviewWindow::OnInitialized()
	{
		return true;
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
	void PreviewWindow::RenderImageControlsBar()
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
	void PreviewWindow::RenderImage()
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
		RenderImageControlsBar();
	}

	constexpr float PLAY_BUTTON_PADDING = 50;

	//---------------------------------------------------------------------
	static int FormatTime(double value, char* buff, int size, void* user_data)
	{
		int totalSeconds = static_cast<int>(value);

		int minutes = totalSeconds / 60;
		int seconds = totalSeconds % 60;

		return snprintf(buff, size, "%d:%02d", minutes, seconds);
	}


	bool m_bIsPlaying = false;
	bool m_bMuted = false;
	//---------------------------------------------------------------------
	void PreviewWindow::RenderAudio()
	{
		resources::Resource* resource = GetWorkspace().GetSelectedResource();
		if (!resource)
		{
			return;
		}

		resources::SoundResource* soundResource = dynamic_cast<resources::SoundResource*>(resource);
		if (!soundResource)
		{
			return;
		}

		core::Data pcmData = soundResource->GetData();
		if (pcmData.empty())
		{
			return;
		}

		const unsigned char* rawSamples = pcmData.dataAs<unsigned char>();
		size_t sampleCount = pcmData.size();
		uint16_t sampleRate = soundResource->GetSampleRate();

		float duration = static_cast<float>(sampleCount) / static_cast<float>(sampleRate);
		m_fAudioDuration = duration;

		// Downsample for plotting if too many samples
		static constexpr int MAX_PLOT_SAMPLES = 4000;
		int plotSamples = (sampleCount > MAX_PLOT_SAMPLES) ? MAX_PLOT_SAMPLES : static_cast<int>(sampleCount);
		float step = static_cast<float>(sampleCount) / static_cast<float>(plotSamples);

		static float x_data[MAX_PLOT_SAMPLES];
		static float y_data[MAX_PLOT_SAMPLES];

		for (int i = 0; i < plotSamples; i++)
		{
			size_t srcIndex = static_cast<size_t>(i * step);
			x_data[i] = static_cast<float>(srcIndex) / static_cast<float>(sampleRate);
			// Convert unsigned 8-bit (0-255) to float (-1.0 to 1.0)
			y_data[i] = (static_cast<float>(rawSamples[srcIndex]) - 127.5f) / 127.5f;
		}

		ImVec2 avail = ImGui::GetContentRegionAvail();
		float controlsBarHeight = ImGui::GetFontSize() + PLAY_BUTTON_PADDING + ImGui::GetStyle().ItemSpacing.y;
		ImVec2 plotSize(avail.x, avail.y - controlsBarHeight);

		if (ImPlot::BeginPlot(FormatId("", PLOT_ID, "WAVEFORM").c_str(), plotSize, ImPlotFlags_NoFrame | ImPlotFlags_NoMouseText))
		{
			ImPlot::SetupAxis(ImAxis_X1, "",
				ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_Opposite
			); 
			ImPlot::SetupAxisFormat(ImAxis_X1, FormatTime, nullptr);
			ImPlot::SetupAxisTicks(ImAxis_Y1, -1.0, 1.0, 5);
			ImPlot::SetupAxisLimits(ImAxis_X1, 0.0, duration, ImPlotCond_Always);

			ImPlot::SetupAxis(ImAxis_Y1, "", ImPlotAxisFlags_NoGridLines);
			ImPlot::SetupAxisFormat(ImAxis_Y1, "%.1f");
			ImPlot::SetupAxisLimits(ImAxis_Y1, -1.0, 1.0, ImPlotCond_Always);

			ImPlot::PlotLine(
				FormatId("", PLOT_ID, "WAVEFORM", "LINE").c_str(),
				x_data,
				y_data,
				plotSamples
			);

			ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(1, 1, 1, 1));
			ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 2);
			ImPlot::PlotInfLines("##Playhead", &m_fAudioPosition, 1);
			ImPlot::PopStyleVar();
			ImPlot::PopStyleColor();

			// Click to seek
			if (ImPlot::IsPlotHovered() && (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseDragging(ImGuiMouseButton_Left)))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_TextInput);

				ImPlotPoint mousePos = ImPlot::GetPlotMousePos();
				m_fAudioPosition = static_cast<float>(mousePos.x);
				if (m_fAudioPosition < 0.0f) m_fAudioPosition = 0.0f;
				if (m_fAudioPosition > m_fAudioDuration) m_fAudioPosition = m_fAudioDuration;
			}

			// Circle on top of the playhead
			ImVec2 plotMin = ImPlot::GetPlotPos();
			ImVec2 plotSize = ImPlot::GetPlotSize();
			float plotTop = plotMin.y;
			ImPlotPoint pos(m_fAudioPosition, 0.0);
			ImVec2 screenPos = ImPlot::PlotToPixels(pos);
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			// Time label above circle
			int posMins = static_cast<int>(m_fAudioPosition) / 60;
			int posSecs = static_cast<int>(m_fAudioPosition) % 60;
			int posMs = static_cast<int>((m_fAudioPosition - floorf(m_fAudioPosition)) * 1000);
			char timeLabel[16];
			snprintf(timeLabel, sizeof(timeLabel), "%d:%02d.%03d", posMins, posSecs, posMs);

			ImVec2 textSize = ImGui::CalcTextSize(timeLabel);
			float padX = 6.0f;
			float padY = 3.0f;
			float boxW = textSize.x + padX * 2.0f;
			float boxH = textSize.y + padY * 2.0f;
			float boxX = screenPos.x - boxW * 0.5f;
			float boxY = plotTop - boxH - 10.0f;

			// Clamp to plot area
			if (boxX < plotMin.x) boxX = plotMin.x;
			if (boxX + boxW > plotMin.x + plotSize.x) boxX = plotMin.x + plotSize.x - boxW;

			ImU32 bgColor = IM_COL32(80, 60, 135, 230);
			drawList->AddRectFilled(
				ImVec2(boxX, boxY),
				ImVec2(boxX + boxW, boxY + boxH),
				bgColor, 4.0f);
			drawList->AddText(
				ImVec2(boxX + padX, boxY + padY),
				IM_COL32(255, 255, 255, 255),
				timeLabel);

			// Small triangle pointer below the label
			float triX = screenPos.x;
			float triY = boxY + boxH;
			drawList->AddTriangleFilled(
				ImVec2(triX - 4.0f, triY),
				ImVec2(triX + 4.0f, triY),
				ImVec2(triX, triY + 5.0f),
				bgColor);

			drawList->AddCircleFilled(ImVec2(screenPos.x, plotTop), 5.0f, IM_COL32(220, 220, 220, 255));

			ImPlot::EndPlot();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 40.0f);
		ImGui::PushStyleColor(ImGuiCol_Button, ImGui::ColorConvertFloat4ToU32(imgui::ExtraColors[imgui::ImGuiExtraCol_Accent]));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::ColorConvertFloat4ToU32(imgui::ExtraColors[imgui::ImGuiExtraCol_AccentHovered]));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::ColorConvertFloat4ToU32(imgui::ExtraColors[imgui::ImGuiExtraCol_AccentActive]));

		const float rowY = ImGui::GetCursorPosY();

		// Play
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(PLAY_BUTTON_PADDING / 2, PLAY_BUTTON_PADDING / 2));

		ImGui::SetCursorPosY(rowY);

		if (TextButton(FormatId(m_bIsPlaying ? icon::ICON_PAUSE : icon::ICON_PLAY, BUTTON_ID, "PLAY").c_str()))
		{
			m_bIsPlaying = !m_bIsPlaying;
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		const float playHeight = ImGui::GetItemRectSize().y;

		ImGui::PopStyleVar();

		// Stop
		float smallBtnSize = ImGui::GetFontSize() + 30.0f;
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15.0f, 15.0f));

		const float smallHeight = ImGui::GetFrameHeight();
		const float offset = (playHeight - smallHeight) * 0.5f;

		ImGui::SameLine();
		ImGui::SetCursorPosY(rowY + offset);

		if (TextButton(FormatId(icon::ICON_STOP, BUTTON_ID, "STOP").c_str(), ImVec2(smallBtnSize, smallBtnSize)))
		{
			m_bIsPlaying = false;
			m_fAudioPosition = 0;
		}

		// Previous
		ImGui::SameLine();
		ImGui::SetCursorPosY(rowY + offset);

		if (TextButton(FormatId(icon::ICON_PREVIOUS, BUTTON_ID, "PREVIOUS").c_str(), ImVec2(smallBtnSize, smallBtnSize)))
		{
			m_bIsPlaying = false;
			m_fAudioPosition -= 0.1f;
		}

		// Next
		ImGui::SameLine();
		ImGui::SetCursorPosY(rowY + offset);

		if (TextButton(FormatId(icon::ICON_NEXT, BUTTON_ID, "NEXT").c_str(), ImVec2(smallBtnSize, smallBtnSize)))
		{
			m_bIsPlaying = false;
			m_fAudioPosition += 0.1f;
		}

		ImGui::PopStyleVar(); // FramePadding
		ImGui::PopStyleVar(); // FrameRounding

		// Next
		ImGui::SameLine();

		// Time label above circle
		int currentPosMinS = static_cast<int>(m_fAudioPosition) / 60;
		int currentPosSecS = static_cast<int>(m_fAudioPosition) % 60;
		int currentPosMs = static_cast<int>((m_fAudioPosition - floorf(m_fAudioPosition)) * 1000);

		// Time label above circle
		int durationMinS = static_cast<int>(m_fAudioDuration) / 60;
		int durationSecS = static_cast<int>(m_fAudioDuration) % 60;
		int durationPosMs = static_cast<int>((m_fAudioDuration - floorf(m_fAudioDuration)) * 1000);

		ImGui::Text("%d:%02d.%03d / %s", currentPosMinS, currentPosSecS, currentPosMs, soundResource->GetDurationStr().c_str());

		// Push knob to the right and vertically centered
		float availWidth = ImGui::GetContentRegionAvail().x;
		float knobSize = 64.0f;
		ImGui::SameLine(availWidth - knobSize);

		Knob(FormatId("", KNOB_ID, "VOLUME").c_str(), &m_fAudioVolume, 0, 1, ImVec2(knobSize, knobSize), 0.0f);

		if (m_bIsPlaying)
		{
			m_fAudioPosition += 0.005f;
		}
		if (m_fAudioPosition >= m_fAudioDuration)
		{
			m_fAudioPosition = 0.0f;
		}
		if (m_fAudioPosition < 0.0f)
		{
			m_fAudioPosition = 0.0f;
		}
	}

	//---------------------------------------------------------------------
	void PreviewWindow::RenderSoundControlsBar()
	{
	}

	//---------------------------------------------------------------------
	void PreviewWindow::RenderInfo()
	{
		resources::Resource* resource = GetWorkspace().GetSelectedResource();
		if (!resource)
		{
			return;
		}

		if (resource->GetResourceType() == resources::ResourceType::Talkie || resource->GetResourceType() == resources::ResourceType::SFX || resource->GetResourceType() == resources::ResourceType::Song)
		{
			RenderAudio();
		}
		//RenderImage();
	}

	//---------------------------------------------------------------------
	void PreviewWindow::Update()
	{
		if (ImGui::BeginChild(
			FormatId("", CHILD_ID, "PREVIEW").c_str(),
			ImVec2(
				ImGui::GetContentRegionAvail().x,
				ImGui::GetContentRegionAvail().y
			),
			ImGuiChildFlags_Borders
		))
		{
			RenderInfo();
		}
		ImGui::EndChild();
	}
}
