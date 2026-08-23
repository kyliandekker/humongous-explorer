#include "PreviewWindow.h"

#include <cmath>
#include <imgui/imgui.h>
#include <imgui/implot.h>
#include <imgui/font_icon.h>
#include <imgui/Helpers.h>
#include <algorithm>

#include "dx11/SVGTextureCache.h"

#include "imgui/ImGuiSystem.h"
#include "editor/Workspace.h"
#include "resources/Resource.h"
#include "core/Data.h"
#include "audio/WaveLoader.h"
#include "file/file.h"
#include "win32/winfile.h"
#include "parsing/chunks/sound/SBNG_Chunk.h"
#include "core/Log.h"

#undef min
#undef max

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
		GetWorkspace().GetSelectedResourceObs().OnChanged() += std::bind(&PreviewWindow::OnSelectedResourceChanged, this, std::placeholders::_1, std::placeholders::_2);
		return true;
	}

	//---------------------------------------------------------------------
	void PreviewWindow::OnSelectedResourceChanged(resources::Resource* oldResource, resources::Resource* newResource)
	{
		m_AudioPlayer.Stop();
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
		float offsetX = (availWidth - totalControlsWidth) * 0.3f;
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

		// Zoom combo â€” shows current zoom as percentage
		char zoomLabel[16];
		snprintf(zoomLabel, sizeof(zoomLabel), "%.0f%%", m_fZoom * 100.0f);
		ImGui::PushItemWidth(90.0f);
		if (ImGui::BeginCombo(FormatId("", DROPDOWN_ID, "PreviewWindow", "Zoom").c_str(), zoomLabel))
		{
			for (int i = 0; i < s_iPresetCount; i++)
			{
				bool isSelected = (fabsf(m_fZoom - s_aPresets[i]) < 0.001f);
				if (ImGui::Selectable(s_aPresetLabels[i], isSelected))
				{
					m_fZoom = s_aPresets[i];
				}
				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
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
		{
			ImGui::PushStyleColor(
				ImGuiCol_Button,
				ImGui::ColorConvertFloat4ToU32(
					imgui::ExtraColors[
						imgui::ImGuiExtraCol_Accent]));
		}

		ImGui::NewLine();

		if (ImGui::ImageButton(FormatId("", BUTTON_ID, "PreviewWindow", "Checkerboard").c_str(),
			(ImTextureID)dx11::SVGTextureCache::Get("icon_transparent.svg"),
			ImVec2(ImGui::GetFontSize() + 2.0f, ImGui::GetFontSize() + 2.0f)))
		{
			m_bShowCheckerboard = !m_bShowCheckerboard;
		}
		if (prevCheckerboard)
		{
			ImGui::PopStyleColor();
		}

		ImGui::PopStyleVar();
	}

	//---------------------------------------------------------------------
	void PreviewWindow::RenderImage()
	{
		ImVec2 avail = ImGui::GetContentRegionAvail();
		float controlsBarHeight = ImGui::GetFrameHeight() + 16.0f;
		ImVec2 previewArea(avail.x, avail.y - controlsBarHeight);

		if (previewArea.x <= 0 || previewArea.y <= 0)
		{
			return;
		}

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
				if (m_fZoom < 0.05f)
				{
					m_fZoom = 0.05f;
				}
				if (m_fZoom > 16.0f)
				{
					m_fZoom = 16.0f;
				}
			}
		}

		// Mouse drag panning when hovering the preview area
		if (ImGui::IsMouseHoveringRect(areaMin, areaMax) && ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
		{
			ImVec2 delta = ImGui::GetIO().MouseDelta;
			m_vPan.x += delta.x;
			m_vPan.y += delta.y;
		}

		resources::Resource* resource = GetWorkspace().GetSelectedResource();
		if (!resource)
		{
			return;
		}

		float imgW = 320.0f;
		float imgH = 200.0f;
		ID3D11ShaderResourceView* srv = nullptr;

		if (resource->GetResourceType() == resources::ResourceType::RoomBackground)
		{
			resources::RoomBackgroundResource* rb = dynamic_cast<resources::RoomBackgroundResource*>(resource);
			imgW = static_cast<float>(rb->GetWidth());
			imgH = static_cast<float>(rb->GetHeight());
			srv = rb->GetSRV();
		}
		else if (resource->GetResourceType() == resources::ResourceType::RoomImage)
		{
			// TODO: support RoomImage
		}

		imgW *= m_fZoom;
		imgH *= m_fZoom;
		float centerX = areaMin.x + previewArea.x * 0.5f + m_vPan.x;
		float centerY = areaMin.y + previewArea.y * 0.5f + m_vPan.y;
		ImVec2 imgMin(centerX - imgW * 0.5f, centerY - imgH * 0.5f);
		ImVec2 imgMax(centerX + imgW * 0.5f, centerY + imgH * 0.5f);

		// Clip to preview area
		drawList->PushClipRect(areaMin, areaMax, true);
		if (srv)
		{
			drawList->AddImage(
				(ImTextureID)srv,
				imgMin,
				imgMax,
				ImVec2(0, 0),
				ImVec2(1, 1)
			);
		}
		else
		{
			drawList->AddRectFilled(imgMin, imgMax, IM_COL32(80, 60, 135, 255));
			drawList->AddRect(imgMin, imgMax, IM_COL32(160, 130, 255, 255));
		}
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

	bool m_bMuted = false;
	//---------------------------------------------------------------------
	void PreviewWindow::RenderAudio()
	{
		resources::Resource* resource = GetWorkspace().GetSelectedResource();
		if (!resource)
		{
			return;
		}

		resources::SoundResource* soundResource =
			dynamic_cast<resources::SoundResource*>(resource);

		if (!soundResource)
		{
			return;
		}

		core::Data pcmData = soundResource->GetData();
		if (pcmData.empty())
		{
			return;
		}

		const uint32_t sampleRate =
			static_cast<uint32_t>(soundResource->GetSampleRate());

		const uint8_t bitsPerSample = 8;
		const uint8_t channels = 1;

		const size_t bytesPerSample = bitsPerSample / 8;
		const size_t bytesPerFrame = bytesPerSample * channels;

		if (sampleRate == 0 || bytesPerFrame == 0)
		{
			return;
		}

		const float fontSize = ImGui::GetFontSize();
		const float playBtnSize = fontSize * 3.0f + 3.0f;
		const float playBtnPad = (playBtnSize - fontSize) * 0.5f;
		const float smallBtnSize = fontSize * 3.0f;
		const float smallBtnPad = (smallBtnSize - fontSize) * 0.5f;
		const float smallRounding = fontSize * 0.4f;
		const float knobSize = smallBtnSize;

		const size_t totalBytes = pcmData.size();

		size_t bytePosition = m_AudioPlayer.GetBytePosition();
		if (bytePosition > totalBytes)
		{
			bytePosition = totalBytes;
		}

		const float bytesPerSecond =
			static_cast<float>(sampleRate * bytesPerFrame);

		const float duration =
			static_cast<float>(totalBytes) / bytesPerSecond;

		float m_fAudioDuration = duration;

		const float currentTime =
			static_cast<float>(bytePosition) / bytesPerSecond;

		const bool isPlaying = m_AudioPlayer.IsPlaying();
		const bool isPaused = m_AudioPlayer.IsPaused();

		const unsigned char* rawSamples =
			pcmData.dataAs<unsigned char>();

		static constexpr int MAX_PLOT_SAMPLES = 4000;

		const int plotSamples =
			(totalBytes > MAX_PLOT_SAMPLES)
			? MAX_PLOT_SAMPLES
			: static_cast<int>(totalBytes);

		if (plotSamples <= 0)
		{
			return;
		}

		const float step =
			static_cast<float>(totalBytes) /
			static_cast<float>(plotSamples);

		static float x_data[MAX_PLOT_SAMPLES];
		static float y_data[MAX_PLOT_SAMPLES];

		for (int i = 0; i < plotSamples; ++i)
		{
			const size_t srcIndex =
				std::min(
					static_cast<size_t>(i * step),
					totalBytes - 1);

			x_data[i] =
				static_cast<float>(srcIndex) / bytesPerSecond;

			y_data[i] =
				(static_cast<float>(rawSamples[srcIndex]) - 127.5f) /
				127.5f;
		}

		ImVec2 avail = ImGui::GetContentRegionAvail();

		const float controlsBarHeight =
			playBtnSize +
			ImGui::GetStyle().ItemSpacing.y;

		ImVec2 plotSize(
			avail.x,
			avail.y - controlsBarHeight);

		if (ImPlot::BeginPlot(
			FormatId("", PLOT_ID, "WAVEFORM").c_str(),
			plotSize,
			ImPlotFlags_NoFrame |
			ImPlotFlags_NoMouseText))
		{
			ImPlot::SetupAxis(
				ImAxis_X1,
				"",
				ImPlotAxisFlags_NoGridLines |
				ImPlotAxisFlags_Opposite);

			ImPlot::SetupAxisFormat(
				ImAxis_X1,
				FormatTime,
				nullptr);

			ImPlot::SetupAxisLimits(
				ImAxis_X1,
				0.0,
				duration,
				ImPlotCond_Always);

			ImPlot::SetupAxis(
				ImAxis_Y1,
				"",
				ImPlotAxisFlags_NoGridLines);

			ImPlot::SetupAxisFormat(
				ImAxis_Y1,
				"%.1f");

			ImPlot::SetupAxisLimits(
				ImAxis_Y1,
				-1.0,
				1.0,
				ImPlotCond_Always);

			ImPlot::PlotLine(
				FormatId(
					"",
					PLOT_ID,
					"WAVEFORM",
					"LINE").c_str(),
				x_data,
				y_data,
				plotSamples);

			float playheadTime = currentTime;

			ImPlot::PushStyleColor(
				ImPlotCol_Line,
				ImVec4(1, 1, 1, 1));

			ImPlot::PushStyleVar(
				ImPlotStyleVar_LineWeight,
				2.0f);

			ImPlot::PlotInfLines(
				FormatId(
					"",
					PLOT_ID,
					"PLAYHEAD").c_str(),
				&playheadTime,
				1);

			ImPlot::PopStyleVar();
			ImPlot::PopStyleColor();

			if (soundResource->GetResourceType() ==
				resources::ResourceType::Talkie)
			{
				resources::TalkResource* talkResource =
					dynamic_cast<resources::TalkResource*>(resource);

				if (talkResource)
				{
					ImPlot::PushStyleColor(
						ImPlotCol_Line,
						ImVec4(1, 0.4f, 0.4f, 0.8f));

					ImPlot::PushStyleVar(
						ImPlotStyleVar_LineWeight,
						1.5f);

					core::Data data =
						talkResource->GetLipSyncData();

					if (!data.empty())
					{
						const parsing::SBNGRecord* records =
							data.dataAs<parsing::SBNGRecord>();

						const size_t recordCount =
							data.size() /
							sizeof(parsing::SBNGRecord);

						for (size_t i = 0; i < recordCount; ++i)
						{
							float xPos =
								static_cast<float>(
									records[i].sampleOffset) /
								static_cast<float>(sampleRate);

							ImPlot::PlotInfLines(
								FormatId(
									"",
									PLOT_ID,
									"LIPSYNC",
									std::to_string(i)).c_str(),
								&xPos,
								1);
						}
					}

					ImPlot::PopStyleVar();
					ImPlot::PopStyleColor();
				}
			}

			if (ImPlot::IsPlotHovered() &&
				(ImGui::IsMouseClicked(ImGuiMouseButton_Left) ||
					ImGui::IsMouseDragging(ImGuiMouseButton_Left)))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_TextInput);

				ImPlotPoint mousePos = ImPlot::GetPlotMousePos();

				const float seekTime =
					std::clamp(
						static_cast<float>(mousePos.x),
						0.0f,
						duration);

				size_t seekBytePosition =
					static_cast<size_t>(
						seekTime * bytesPerSecond);

				seekBytePosition =
					(seekBytePosition / bytesPerFrame) *
					bytesPerFrame;

				m_AudioPlayer.Seek(seekBytePosition);

				bytePosition =
					m_AudioPlayer.GetBytePosition();
			}

			ImVec2 plotMin = ImPlot::GetPlotPos();
			ImVec2 actualPlotSize = ImPlot::GetPlotSize();

			const float plotTop = plotMin.y;

			ImPlotPoint pos(currentTime, 0.0);
			ImVec2 screenPos = ImPlot::PlotToPixels(pos);

			ImDrawList* drawList =
				ImGui::GetWindowDrawList();

			int posMins =
				static_cast<int>(currentTime) / 60;

			int posSecs =
				static_cast<int>(currentTime) % 60;

			int posMs =
				static_cast<int>(
					(currentTime - std::floor(currentTime)) *
					1000.0f);

			char timeLabel[32];

			snprintf(
				timeLabel,
				sizeof(timeLabel),
				"%d:%02d.%03d",
				posMins,
				posSecs,
				posMs);

			ImVec2 textSize =
				ImGui::CalcTextSize(timeLabel);

			const float padX = 6.0f;
			const float padY = 3.0f;

			const float boxW =
				textSize.x + padX * 2.0f;

			const float boxH =
				textSize.y + padY * 2.0f;

			float boxX =
				screenPos.x - boxW * 0.5f;

			const float boxY =
				plotTop - boxH - 10.0f;

			if (boxX < plotMin.x)
			{
				boxX = plotMin.x;
			}

			if (boxX + boxW >
				plotMin.x + actualPlotSize.x)
			{
				boxX =
					plotMin.x +
					actualPlotSize.x -
					boxW;
			}

			ImU32 bgColor =
				IM_COL32(80, 60, 135, 230);

			drawList->AddRectFilled(
				ImVec2(boxX, boxY),
				ImVec2(boxX + boxW, boxY + boxH),
				bgColor,
				4.0f);

			drawList->AddText(
				ImVec2(
					boxX + padX,
					boxY + padY),
				IM_COL32(255, 255, 255, 255),
				timeLabel);

			const float triX = screenPos.x;
			const float triY = boxY + boxH;

			drawList->AddTriangleFilled(
				ImVec2(triX - 4.0f, triY),
				ImVec2(triX + 4.0f, triY),
				ImVec2(triX, triY + 5.0f),
				bgColor);

			drawList->AddCircleFilled(
				ImVec2(screenPos.x, plotTop),
				5.0f,
				IM_COL32(220, 220, 220, 255));

			ImPlot::EndPlot();
		}

		ImGui::PushStyleVar(
			ImGuiStyleVar_FrameRounding,
			playBtnSize / 2.0f);

		ImGui::PushStyleColor(
			ImGuiCol_Button,
			ImGui::ColorConvertFloat4ToU32(
				imgui::ExtraColors[
					imgui::ImGuiExtraCol_Accent]));

		ImGui::PushStyleColor(
			ImGuiCol_ButtonHovered,
			ImGui::ColorConvertFloat4ToU32(
				imgui::ExtraColors[
					imgui::ImGuiExtraCol_AccentHovered]));

		ImGui::PushStyleColor(
			ImGuiCol_ButtonActive,
			ImGui::ColorConvertFloat4ToU32(
				imgui::ExtraColors[
					imgui::ImGuiExtraCol_AccentActive]));

		const float rowY = ImGui::GetCursorPosY();

		ImGui::PushStyleVar(
			ImGuiStyleVar_FramePadding,
			ImVec2(
				playBtnPad,
				playBtnPad));

		ImGui::SetCursorPosY(rowY);

		const char* playIcon =
			(!isPlaying || isPaused)
			? icon::ICON_PLAY
			: icon::ICON_PAUSE;

		if (TextButton(
			FormatId(
				playIcon,
				BUTTON_ID,
				"PLAY").c_str(), ImVec2(playBtnSize, playBtnSize)))
		{
			if (isPlaying)
			{
				m_AudioPlayer.Pause();
			}
			else if (isPaused)
			{
				m_AudioPlayer.Resume();
			}
			else
			{
				if (!m_AudioPlayer.IsOpen())
				{
					m_AudioPlayer.Open(
						sampleRate,
						bitsPerSample,
						channels);
				}

				m_AudioPlayer.Play(
					pcmData.data(),
					pcmData.size());
			}
		}

		ImGui::PopStyleVar();

		const float playHeight =
			ImGui::GetItemRectSize().y;

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		ImGui::PushStyleVar(
			ImGuiStyleVar_FrameRounding,
			smallRounding);

		ImGui::PushStyleVar(
			ImGuiStyleVar_FramePadding,
			ImVec2(smallBtnPad, smallBtnPad));

		const float smallHeight =
			ImGui::GetFrameHeight();

		const float offset =
			(playHeight - smallHeight) * 0.5f;

		ImGui::SameLine();
		ImGui::SetCursorPosY(rowY + offset);

		if (TextButton(
			FormatId(
				icon::ICON_STOP,
				BUTTON_ID,
				"STOP").c_str(),
			ImVec2(
				smallBtnSize,
				smallBtnSize)))
		{
			m_AudioPlayer.Stop();
		}

		ImGui::SameLine();
		ImGui::SetCursorPosY(rowY + offset);

		if (TextButton(
			FormatId(
				icon::ICON_PREVIOUS,
				BUTTON_ID,
				"PREVIOUS").c_str(),
			ImVec2(
				smallBtnSize,
				smallBtnSize)))
		{
			const size_t stepBytes =
				static_cast<size_t>(
					sampleRate *
					bytesPerFrame *
					0.1f);

			size_t newPosition =
				m_AudioPlayer.GetBytePosition();

			if (newPosition > stepBytes)
			{
				newPosition -= stepBytes;
			}
			else
			{
				newPosition = 0;
			}

			newPosition =
				(newPosition / bytesPerFrame) *
				bytesPerFrame;

			m_AudioPlayer.Seek(newPosition);
		}

		ImGui::SameLine();
		ImGui::SetCursorPosY(rowY + offset);

		if (TextButton(
			FormatId(
				icon::ICON_NEXT,
				BUTTON_ID,
				"NEXT").c_str(),
			ImVec2(
				smallBtnSize,
				smallBtnSize)))
		{
			const size_t stepBytes =
				static_cast<size_t>(
					sampleRate *
					bytesPerFrame *
					0.1f);

			size_t newPosition =
				m_AudioPlayer.GetBytePosition();

			newPosition =
				std::min(
					newPosition + stepBytes,
					totalBytes);

			newPosition =
				(newPosition / bytesPerFrame) *
				bytesPerFrame;

			m_AudioPlayer.Seek(newPosition);
		}

		ImGui::PopStyleVar();
		ImGui::PopStyleVar();

		ImGui::SameLine();

		bytePosition =
			m_AudioPlayer.GetBytePosition();

		const float displayTime =
			static_cast<float>(bytePosition) /
			bytesPerSecond;

		const int currentPosMinS =
			static_cast<int>(displayTime) / 60;

		const int currentPosSecS =
			static_cast<int>(displayTime) % 60;

		const int currentPosMs =
			static_cast<int>(
				(displayTime - std::floor(displayTime)) *
				1000.0f);

		const int durationMinS =
			static_cast<int>(duration) / 60;

		const int durationSecS =
			static_cast<int>(duration) % 60;

		const int durationPosMs =
			static_cast<int>(
				(duration - std::floor(duration)) *
				1000.0f);

		ImGui::Text(
			"%d:%02d.%03d / %d:%02d.%03d",
			currentPosMinS,
			currentPosSecS,
			currentPosMs,
			durationMinS,
			durationSecS,
			durationPosMs);

		const float availWidth =
			ImGui::GetContentRegionAvail().x;

		ImGui::SameLine(
			availWidth - (knobSize + smallBtnSize + smallBtnSize));

		ImGui::SetCursorPosY(rowY + offset);
		if (TextButton(
			FormatId(
				icon::ICON_SAVE,
				BUTTON_ID,
				"SAVE").c_str(),
			ImVec2(
				smallBtnSize,
				smallBtnSize)))
		{
			//fs::path savePath;
			//const std::vector<COMDLG_FILTERSPEC> filters =
			//{
			//	{L"WAVE files (*.wav)", L"*.wav;*.WAV"}
			//};
			//if (file::SaveFile(savePath, filters))
			//{
			//	if (file::SaveFile(savePath, ))
			//	{
			//		core::Log(core::LogLevel::Success, "Saved file to \"" + savePath.string() + "\".");
			//	}
			//	else
			//	{
			//		core::Log(core::LogLevel::Error, "Failed to save file to \"" + savePath.string() + "\".");
			//	}
			//}
			//else
			//{
			//	core::Log(core::LogLevel::Error, "Failed to save file to \"" + savePath.string() + "\".");
			//}
		}

		ImGui::SameLine();

		ImGui::SetCursorPosY(rowY + offset);
		if (TextButton(
			FormatId(
				icon::ICON_LOAD,
				BUTTON_ID,
				"LOAD").c_str(),
			ImVec2(
				smallBtnSize,
				smallBtnSize)))
		{
			fs::path savePath;
			const std::vector<COMDLG_FILTERSPEC> filters =
			{
				{L"WAVE files (*.wav)", L"*.wav;*.WAV"}
			};
			if (file::PickFile(savePath, filters))
			{
				core::Data data;
				uint16_t sampleRate;
				if (audio::WaveLoader::Load(savePath.string(), data, sampleRate))
				{
					soundResource->Replace(data);
				}
			}
		}

		ImGui::SameLine();

		ImGui::SetCursorPosY(rowY + offset);
		Knob(
			FormatId(
				"",
				KNOB_ID,
				"VOLUME").c_str(),
			&m_fAudioVolume,
			0,
			1,
			ImVec2(knobSize, knobSize),
			0.0f
		);
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
		else if (resource->GetResourceType() == resources::ResourceType::RoomBackground || resource->GetResourceType() == resources::ResourceType::RoomImage)
		{
			RenderImage();
		}
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