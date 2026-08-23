#include "TopToolbarWindow.h"

#include <imgui/imgui.h>
#include <imgui/Helpers.h>
#include <imgui/font_icon.h>

#include "win32/Window.h"

#include "dx11/SVGTextureCache.h"
#include "editor/Workspace.h"

#include "imgui/ImGuiSystem.h"

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	TopToolbarWindow::TopToolbarWindow()
		: HEBaseWindow(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse, "", "TOOLBAR", true)
	{
	}

	//---------------------------------------------------------------------
	void TopToolbarWindow::Update()
	{
		ImDrawList* drawlist = ImGui::GetWindowDrawList();
		ImVec2 topToolbarStart = ImGui::GetWindowPos();
		ImVec2 topToolbarEnd = { topToolbarStart.x + ImGui::GetWindowWidth(), topToolbarStart.y + TOP_TOOLBAR_HEIGHT };
		drawlist->AddRectFilled(
			ImGui::GetWindowPos(),
			ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(),
				ImGui::GetWindowPos().y + TOP_TOOLBAR_HEIGHT),
			IM_COL32(21, 26, 36, 255)
		);

		// Logo + app name on left side of top toolbar
		{
			ImVec2 padding = ImGui::GetStyle().WindowPadding;
			std::string appName = GetWorkspace().GetAppName();
			ImVec2 textSize = ImGui::CalcTextSize(appName.c_str());

			const float logoIconSize = 48.0f;
			ID3D11ShaderResourceView* logoTex = dx11::SVGTextureCache::Get("icon_logo.svg");
			float logoDrawW = 0.0f;
			float logoDrawH = 0.0f;
			if (logoTex)
			{
				int nativeW = dx11::SVGTextureCache::GetWidth("icon_logo.svg");
				int nativeH = dx11::SVGTextureCache::GetHeight("icon_logo.svg");
				if (nativeW > 0 && nativeH > 0)
				{
					float scale = logoIconSize / static_cast<float>((nativeW > nativeH) ? nativeW : nativeH);
					logoDrawW = nativeW * scale;
					logoDrawH = nativeH * scale;
				}
				else
				{
					logoTex = nullptr;
				}
			}

			float logoSpacing = logoTex ? 8.0f : 0.0f;
			ImVec2 logoPos = {
				topToolbarStart.x + padding.x,
				topToolbarStart.y + (TOP_TOOLBAR_HEIGHT - logoDrawH) * 0.5f
			};
			ImVec2 textPos = {
				topToolbarStart.x + padding.x + logoDrawW + logoSpacing,
				topToolbarStart.y + (TOP_TOOLBAR_HEIGHT - textSize.y) * 0.5f
			};

			if (logoTex)
			{
				drawlist->AddImage(
					static_cast<ImTextureID>(reinterpret_cast<intptr_t>(logoTex)),
					logoPos,
					ImVec2(logoPos.x + logoDrawW, logoPos.y + logoDrawH));
			}

			ImGui::PushFont(GetImGuiSystem().GetBoldFont());
			ImGui::SetCursorScreenPos(textPos);
			ImGui::TextUnformatted(appName.c_str());
			ImGui::PopFont();
		}

		ImVec2 Size = ImGui::CalcTextSize(icon::ICON_MINUS);
		Size.y += ImGui::GetStyle().FramePadding.y * 2.0f;
		Size.x += ImGui::GetStyle().FramePadding.x * 2;
		Size.x *= 3;
		Size.x += ImGui::GetStyle().ItemSpacing.x * 2;
		Size.x += ImGui::GetStyle().WindowPadding.x;

		float buttonHeight = ImGui::GetFrameHeight();
		ImVec2 buttonPos = { topToolbarEnd.x - Size.x, topToolbarStart.y + (TOP_TOOLBAR_HEIGHT - Size.y) * 0.5f };
		ImGui::SetCursorScreenPos(buttonPos);
		if (ImGui::Button(FormatId(icon::ICON_MINUS, BUTTON_ID, "MINIMIZE").c_str()))
		{
			win32::GetWin32Window().Minimize();
		}
		ImGui::SameLine();
		if (ImGui::Button(FormatId(icon::ICON_MAXIMIZE, BUTTON_ID, "MAXIMIZE").c_str()))
		{
			win32::GetWin32Window().Maximize();
		}
		ImGui::SameLine();
		if (ImGui::Button(FormatId(icon::ICON_CLOSE, BUTTON_ID, "CLOSE").c_str()))
		{
			win32::GetWin32Window().Close();
		}

		TOP_TOOLBAR_HEIGHT = Size.y + (ImGui::GetStyle().WindowPadding.y * 2);
		BUTTONS_TOOLBAR_WIDTH = Size.x;
	}

	//---------------------------------------------------------------------
	bool TopToolbarWindow::OnInitialized()
	{
		return true;
	}
}
