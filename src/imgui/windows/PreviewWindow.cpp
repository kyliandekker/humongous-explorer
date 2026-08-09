#include "PreviewWindow.h"

#include <cmath>
#include <imgui/imgui.h>
#include <imgui/implot.h>
#include <imgui/icon.h>
#include <imgui/Helpers.h>
#include <algorithm>

#include "dx11/SVGTextureCache.h"

#include "imgui/ImGuiSetup.h"
#include "imgui/Helpers.h"

namespace humongousexplorer::imgui
{
	const float PreviewWindow::s_aPresets[s_iPresetCount] = { 0.25f, 0.50f, 0.75f, 1.0f, 1.5f, 2.0f };
	const char* PreviewWindow::s_aPresetLabels[s_iPresetCount] = { "25%", "50%", "75%", "100%", "150%", "200%" };

	//---------------------------------------------------------------------
	PreviewWindow::PreviewWindow() : HEBaseWindow(ImGuiWindowFlags_NoCollapse, "PREVIEW", "PreviewWindow")
	{}

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

	//---------------------------------------------------------------------
	static int FormatTime(double value, char* buff, int size, void* user_data)
	{
		int totalSeconds = static_cast<int>(value);

		int minutes = totalSeconds / 60;
		int seconds = totalSeconds % 60;

		return snprintf(buff, size, "%d:%02d", minutes, seconds);
	}

	//---------------------------------------------------------------------
	void PreviewWindow::RenderAudio()
	{
		static constexpr int SAMPLE_COUNT = 2000;
		static constexpr float DURATION = 83.456f;

		static float x_data[SAMPLE_COUNT];
		static float y_data[SAMPLE_COUNT];
		static bool s_bInitialized = false;

		if (!s_bInitialized)
		{
			// Deterministic pseudo-random noise.
			uint32_t seed = 0x12345678;

			auto noise = [&]()
				{
					seed = seed * 1664525u + 1013904223u;
					return (static_cast<float>(seed & 0xFFFF) / 32767.5f) - 1.0f;
				};

			for (int i = 0; i < SAMPLE_COUNT; i++)
			{
				float t = static_cast<float>(i) / (SAMPLE_COUNT - 1);
				float time = t * DURATION;

				x_data[i] = time;

				// Several "spoken phrases" throughout the recording.
				float envelope = 0.0f;

				struct Segment
				{
					float start;
					float end;
					float amplitude;
				};

				static constexpr Segment segments[] =
				{
					{  1.0f,  6.5f, 0.75f },
					{  8.0f, 14.0f, 0.55f },
					{ 16.0f, 22.5f, 0.80f },
					{ 25.0f, 29.0f, 0.45f },
					{ 31.0f, 39.0f, 0.72f },
					{ 41.5f, 47.0f, 0.58f },
					{ 49.0f, 56.5f, 0.82f },
					{ 59.0f, 64.0f, 0.50f },
					{ 66.0f, 73.0f, 0.70f },
					{ 75.0f, 81.5f, 0.62f },
				};

				for (const auto& s : segments)
				{
					if (time >= s.start && time <= s.end)
					{
						float local = (time - s.start) / (s.end - s.start);

						// Smooth attack/release.
						float fadeIn = std::min(local / 0.12f, 1.0f);
						float fadeOut = std::min((1.0f - local) / 0.15f, 1.0f);
						float fade = fadeIn * fadeOut;

						// Changing syllable-like amplitude.
						float syllables =
							0.55f +
							0.30f * sinf(time * 7.0f) +
							0.15f * sinf(time * 13.0f);

						envelope = std::max(
							envelope,
							s.amplitude * fade * syllables
						);
					}
				}

				// Fundamental-ish voice component.
				float voice =
					sinf(time * 2.0f * 3.14159f * 2.2f) +
					0.45f * sinf(time * 2.0f * 3.14159f * 4.7f) +
					0.20f * sinf(time * 2.0f * 3.14159f * 8.3f);

				// Higher frequency content.
				float harmonics =
					0.18f * sinf(time * 2.0f * 3.14159f * 17.0f) +
					0.10f * sinf(time * 2.0f * 3.14159f * 31.0f);

				// Low-level noise makes it look less synthetic.
				float noiseFloor = noise() * 0.025f;

				float sample =
					envelope * (voice * 0.42f + harmonics)
					+ noiseFloor;

				y_data[i] = std::clamp(sample, -1.0f, 1.0f);
			}

			s_bInitialized = true;
		}

		ImVec2 avail = ImGui::GetContentRegionAvail();
		float controlsBarHeight = ImGui::GetFrameHeight();
		ImVec2 plotSize(avail.x, avail.y - controlsBarHeight);

		if (ImPlot::BeginPlot(FormatId("", PLOT_ID, "WAVEFORM").c_str(), plotSize, ImPlotFlags_NoFrame | ImPlotFlags_NoMouseText))
		{
			ImPlot::SetupAxis(ImAxis_X1, "",
				ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_Opposite
			); 
			ImPlot::SetupAxisFormat(ImAxis_X1, FormatTime, nullptr);
			ImPlot::SetupAxisTicks(ImAxis_Y1, -1.0, 1.0, 5);
			ImPlot::SetupAxisLimits(ImAxis_X1, 0.0, DURATION, ImPlotCond_Always);

			ImPlot::SetupAxis(ImAxis_Y1, "", ImPlotAxisFlags_NoGridLines);
			ImPlot::SetupAxisFormat(ImAxis_Y1, "%.1f");
			ImPlot::SetupAxisLimits(ImAxis_Y1, -1.0, 1.0, ImPlotCond_Always);

			ImPlot::PlotLine(
				FormatId("", PLOT_ID, "WAVEFORM", "LINE").c_str(),
				x_data,
				y_data,
				SAMPLE_COUNT
			);

			ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(1, 1, 1, 1));
			ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 2);
			ImPlot::PlotInfLines("##Playhead", &m_fAudioPosition, 1);
			ImPlot::PopStyleVar();
			ImPlot::PopStyleColor();

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

		// Controls bar at the bottom
		RenderSoundControlsBar();
	}

	//---------------------------------------------------------------------
	void PreviewWindow::RenderSoundControlsBar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 40.0f);
		ImGui::PushStyleColor(ImGuiCol_Button, ImGui::ColorConvertFloat4ToU32(imgui::ExtraColors[imgui::ImGuiExtraCol_Accent]));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::ColorConvertFloat4ToU32(imgui::ExtraColors[imgui::ImGuiExtraCol_AccentHovered]));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::ColorConvertFloat4ToU32(imgui::ExtraColors[imgui::ImGuiExtraCol_AccentActive]));

		const float rowY = ImGui::GetCursorPosY();

		// Play
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(25.0f, 25.0f));

		ImGui::SetCursorPosY(rowY);

		if (TextButton(FormatId(icon::ICON_PLAY, BUTTON_ID, "PLAY").c_str()))
		{
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		const float playHeight = ImGui::GetItemRectSize().y;

		ImGui::PopStyleVar();

		// Stop
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15.0f, 15.0f));

		const float smallHeight = ImGui::GetFrameHeight();
		const float offset = (playHeight - smallHeight) * 0.5f;

		ImGui::SameLine();
		ImGui::SetCursorPosY(rowY + offset);

		if (TextButton(FormatId(icon::ICON_STOP, BUTTON_ID, "STOP").c_str()))
		{
		}

		// Previous
		ImGui::SameLine();
		ImGui::SetCursorPosY(rowY + offset);

		if (TextButton(FormatId(icon::ICON_PREVIOUS, BUTTON_ID, "PREVIOUS").c_str()))
		{
		}

		// Next
		ImGui::SameLine();
		ImGui::SetCursorPosY(rowY + offset);

		if (TextButton(FormatId(icon::ICON_NEXT, BUTTON_ID, "NEXT").c_str()))
		{
		}

		ImGui::PopStyleVar();
	}

	//---------------------------------------------------------------------
	void PreviewWindow::Update()
	{
		m_fAudioPosition += 0.05f;
		if (m_fAudioPosition >= m_fAudioDuration)
			m_fAudioPosition = 0.0f;

		RenderAudio();
		//RenderImage();
	}
}
