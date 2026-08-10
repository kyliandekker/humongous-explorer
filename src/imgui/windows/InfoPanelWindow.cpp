#include "./InfoPanelWindow.h"

#include <imgui/imgui.h>

#include "imgui/ImGuiSetup.h"
#include "imgui/Helpers.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	InfoPanelWindow::InfoPanelWindow() : HEBaseWindow(ImGuiWindowFlags_NoCollapse, "INFO", "InfoPanelWindow")
	{
	}

	//---------------------------------------------------------------------
	bool InfoPanelWindow::Initialize()
	{
		return HEBaseWindow::Initialize();
	}

	//---------------------------------------------------------------------
	void InfoPanelWindow::Update()
	{
		ImVec2 avail = ImGui::GetContentRegionAvail();
		float spacing = ImGui::GetStyle().ItemSpacing.y;
		float thirdHeight = (avail.y - spacing * 2) / 3.0f;

		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f);

		// Sound Information child
		if (ImGui::BeginChild("SoundInfo", ImVec2(avail.x, thirdHeight), ImGuiChildFlags_Borders))
		{
			ImGui::PushFont(GetBoldFont());
			ImGui::Text("SOUND INFORMATION");
			ImGui::PopFont();
			ImGui::Separator();
			ImGui::Text("Name: THEME.HE0");
			ImGui::Text("Type: Sound");
			ImGui::Text("Size: 345.7 KB");
			ImGui::Text("Format: HE Sound (HE0)");
			ImGui::Text("Channels: 1 (Mono)");
			ImGui::Text("Sample Rate: 22050 Hz");
			ImGui::Text("Duration: 1:23.456");
		}
		ImGui::EndChild();

		// Room Information child
		if (ImGui::BeginChild("RoomInfo", ImVec2(avail.x, thirdHeight), ImGuiChildFlags_Borders))
		{
			ImGui::PushFont(GetBoldFont());
			ImGui::Text("ROOM INFORMATION");
			ImGui::PopFont();
			ImGui::Separator();
			ImGui::Text("Number: 001");
			ImGui::Text("Name: Front Yard");
			ImGui::Text("Image Count: 42");
			ImGui::Text("Sound Count: 18");
			ImGui::Text("SFX Count: 24");
			ImGui::Text("Talkie Count: 6");
			ImGui::Text("Script Count: 4");
		}
		ImGui::EndChild();

		// Palette child
		if (ImGui::BeginChild("Palette", ImVec2(avail.x, thirdHeight), ImGuiChildFlags_Borders))
		{
			// Header row: title on left, count + buttons on right
			ImGui::PushFont(GetBoldFont());
			ImGui::Text("PALETTE");
			ImGui::PopFont();

			ImGui::SameLine();
			float rightWidth = ImGui::CalcTextSize("256 colors").x;
			ImGui::SetCursorPosX(avail.x - rightWidth - ImGui::GetStyle().WindowPadding.x * 2.0f);

			ImGui::Text("256 colors");

			ImGui::Separator();

			// Color swatches
			float swatchSize = ImGui::GetFontSize() + 6.0f;
			float availSwatchWidth = avail.x - ImGui::GetStyle().WindowPadding.x * 2.0f;
			int swatchesPerRow = static_cast<int>(availSwatchWidth / (swatchSize + ImGui::GetStyle().ItemSpacing.x));

			// Generate a demo palette (256-color VGA-style)
			static ImVec4 palette[32];
			static bool paletteInit = false;
			if (!paletteInit)
			{
				for (int i = 0; i < 32; i++)
				{
					float hue = static_cast<float>(i) / 32.0f;
					float sat = (i % 4 < 2) ? 0.7f : 0.4f;
					float val = (i % 2 == 0) ? 0.8f : 0.6f;
					ImVec4 color;
					ImGui::ColorConvertHSVtoRGB(hue, sat, val, color.x, color.y, color.z);
					color.w = 1.0f;
					palette[i] = color;
				}
				paletteInit = true;
			}

			for (int i = 0; i < 32; i++)
			{
				if (i > 0 && (i % swatchesPerRow) != 0)
					ImGui::SameLine();

				ImDrawList* drawList = ImGui::GetWindowDrawList();
				ImVec2 pos = ImGui::GetCursorScreenPos();
				ImVec2 size(swatchSize, swatchSize);

				// Draw swatch
				drawList->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y),
					ImGui::ColorConvertFloat4ToU32(palette[i]), 2.0f);
				drawList->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y),
					IM_COL32(60, 68, 85, 255), 2.0f);

				// Invisible button for interaction
				ImGui::InvisibleButton(FormatId("", BUTTON_ID, "InfoPanelWindow", "Swatch", std::to_string(i)).c_str(), size);
			}
		}
		ImGui::EndChild();

		ImGui::PopStyleVar();
	}
}