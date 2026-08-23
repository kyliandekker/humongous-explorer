#include "InfoPanelWindow.h"

#include <imgui/imgui.h>

#include "imgui/ImGuiSystem.h"
#include "imgui/Helpers.h"

#include "dx11/SVGTextureCache.h"

#include "core/Memory.h"
#include "editor/Workspace.h"
#include "resources/Resource.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	InfoPanelWindow::InfoPanelWindow() : HEBaseWindow(ImGuiWindowFlags_NoCollapse, "INFO", "InfoPanelWindow")
	{
	}

	//---------------------------------------------------------------------
	bool InfoPanelWindow::OnInitialized()
	{
		return true;
	}

	//---------------------------------------------------------------------
	void InfoPanelWindow::Update()
	{
		ImVec2 avail = ImGui::GetContentRegionAvail();
		float spacing = ImGui::GetStyle().ItemSpacing.y;

		resources::Resource* resource = GetWorkspace().GetSelectedResource();
		if (!resource)
		{
			return;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f);

		if (resource->GetResourceType() == resources::ResourceType::Talkie || resource->GetResourceType() == resources::ResourceType::SFX || resource->GetResourceType() == resources::ResourceType::Song)
		{
			resources::SoundResource* soundResource = dynamic_cast<resources::SoundResource*>(resource);

			// Sound Information child
			if (ImGui::BeginChild("SoundInfo", ImVec2(0, 0), ImGuiChildFlags_Borders))
			{
				ImGui::PushFont(GetImGuiSystem().GetBoldFont());
				ImGui::Text("SOUND INFORMATION");
				ImGui::PopFont();
				ImGui::Separator();
				ImGui::Text("Name: %s", soundResource->GetName().c_str());
				ImGui::Text("Type: Sound");
				ImGui::Text("Size: %s", soundResource->GetSize().c_str());
				ImGui::Text("Channels: 1 (Mono)");
				ImGui::Text("Sample Rate: %i Hz", soundResource->GetSampleRate());
				ImGui::Text("Bits per Sample: %i", 8);

				size_t bytesPerSecond =
					(size_t)soundResource->GetSampleRate() * 1 * (8 / 8);

				size_t totalMs =
					(soundResource->GetData().size() * 1000) / bytesPerSecond;

				size_t hours = totalMs / 3600000;
				size_t minutes = (totalMs % 3600000) / 60000;
				size_t seconds = (totalMs % 60000) / 1000;
				size_t ms = totalMs % 1000;

				ImGui::Text("Duration: %s", soundResource->GetDurationStr().c_str());

				if (resource->GetResourceType() == resources::ResourceType::Talkie)
				{
					resources::TalkResource* talkResource = dynamic_cast<resources::TalkResource*>(resource);
					ImGui::Text("Has Lip Sync: %s", talkResource->GetLipSyncData().empty() ? "No" : "Yes");
				}
			}
			ImGui::EndChild();
		}
		if (resource->GetResourceType() == resources::ResourceType::RoomBackground)
		{
			resources::RoomBackgroundResource* roomBackgroundResource = dynamic_cast<resources::RoomBackgroundResource*>(resource);

			// Palette child
			if (ImGui::BeginChild("Palette", ImVec2(0, 0), ImGuiChildFlags_Borders))
			{
				// Header row: icon + title on left, count on right
				float iconSize = ImGui::GetFontSize();
				ID3D11ShaderResourceView* paletteIcon = dx11::SVGTextureCache::Get("icon_palette.svg");
				if (paletteIcon)
				{
					ImGui::Image((ImTextureID)paletteIcon, ImVec2(iconSize, iconSize));
					ImGui::SameLine();
				}
				ImGui::PushFont(GetImGuiSystem().GetBoldFont());
				ImGui::Text("PALETTE");
				ImGui::PopFont();

				const std::vector<Color>& colors = roomBackgroundResource->GetColors();

				std::string numColors = std::to_string(colors.size()) + " colors";

				ImGui::SameLine();
				float rightWidth = ImGui::CalcTextSize(numColors.c_str()).x;
				ImGui::SetCursorPosX(avail.x - rightWidth - ImGui::GetStyle().WindowPadding.x * 2.0f);

				ImGui::Text("%s", numColors.c_str());

				ImGui::Separator();

				// Color swatches
				float swatchSize = ImGui::GetFontSize() + 6.0f;
				float availSwatchWidth = avail.x - ImGui::GetStyle().WindowPadding.x * 2.0f;
				int swatchesPerRow = static_cast<int>(
					availSwatchWidth / (swatchSize + ImGui::GetStyle().ItemSpacing.x)
					);

				if (swatchesPerRow == 0)
				{
					swatchesPerRow = 2;
				}

				for (size_t i = 0; i < colors.size(); ++i)
				{
					if (i > 0 && (i % swatchesPerRow) != 0)
					{
						ImGui::SameLine();
					}

					ImDrawList* drawList = ImGui::GetWindowDrawList();
					ImVec2 pos = ImGui::GetCursorScreenPos();
					ImVec2 size(swatchSize, swatchSize);

					const Color& color = colors[i];

					ImVec4 imguiColor(
						color.r / 255.0f,
						color.g / 255.0f,
						color.b / 255.0f,
						1.0f
					);

					// Draw swatch
					drawList->AddRectFilled(
						pos,
						ImVec2(pos.x + size.x, pos.y + size.y),
						ImGui::ColorConvertFloat4ToU32(imguiColor),
						2.0f
					);

					drawList->AddRect(
						pos,
						ImVec2(pos.x + size.x, pos.y + size.y),
						IM_COL32(60, 68, 85, 255),
						2.0f
					);

					// Invisible button for interaction
					ImGui::InvisibleButton(
						FormatId(
							"",
							BUTTON_ID,
							"InfoPanelWindow",
							"Swatch",
							std::to_string(i)
						).c_str(),
						size
					);
				}
			}
			ImGui::EndChild();
		}

		ImGui::PopStyleVar();
	}
}