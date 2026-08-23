#include "BottomToolbarWindow.h"

#include <imgui/imgui.h>
#include <imgui/Helpers.h>

#include "win32/Window.h"

#include "ui/AppLogger.h"

#include "editor/Workspace.h"

#include "dx11/SVGTextureCache.h"

#include "imgui/ImGuiSystem.h"
#include "imgui/windows/TopToolbarWindow.h"

namespace humongousexplorer::imgui
{
	constexpr float LOG_PANEL_HEIGHT = 200;

	//---------------------------------------------------------------------
	// BottomToolbarWindow
	//---------------------------------------------------------------------
	BottomToolbarWindow::BottomToolbarWindow()
		: HEBaseWindow(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse, "", "TOOLBAR", true)
	{
	}

	//---------------------------------------------------------------------
	void BottomToolbarWindow::Update()
	{
		ImDrawList* drawlist = ImGui::GetWindowDrawList();
		ImVec2 bottomToolbarStart = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y - BOTTOM_TOOLBAR_HEIGHT };
		ImVec2 bottomToolbarEnd = { bottomToolbarStart.x + ImGui::GetWindowSize().x, bottomToolbarStart.y + BOTTOM_TOOLBAR_HEIGHT };
		drawlist->AddRectFilled(
			bottomToolbarStart,
			bottomToolbarEnd,
			IM_COL32(21, 26, 36, 255)
		);

		ImVec2 padding = ImGui::GetStyle().WindowPadding;
		std::string version = GetWorkspace().GetAppName() + " " + GetWorkspace().GetAppVersion();
		ImVec2 versionTextSize = ImGui::CalcTextSize(version.c_str());
		ImVec2 versionTextPos = {
			bottomToolbarEnd.x - (padding.x + versionTextSize.x),
			bottomToolbarStart.y + (BOTTOM_TOOLBAR_HEIGHT - versionTextSize.y) * 0.5f
		};
		ImGui::SetCursorScreenPos(versionTextPos);
		ImGui::Text("%s", version.c_str());

		if (m_aLogHistory.empty())
		{
			return;
		}

		LogEntry& lastEntry = m_aLogHistory[m_aLogHistory.size() - 1];

		const float iconSize = 24.0f;
		float cursorX = bottomToolbarStart.x + padding.x;

		// Track icon rect for click detection
		bool hasIcon = false;
		ImVec2 iconMin = {};
		ImVec2 iconMax = {};
		float iconDrawW = 0.0f;
		float iconDrawH = 0.0f;

		const std::string& icon = lastEntry.icon;
		if (!icon.empty())
		{
			ID3D11ShaderResourceView* tex = dx11::SVGTextureCache::Get(icon);
			if (tex)
			{
				int nativeW = dx11::SVGTextureCache::GetWidth(icon);
				int nativeH = dx11::SVGTextureCache::GetHeight(icon);
				if (nativeW > 0 && nativeH > 0)
				{
					float scale = iconSize / static_cast<float>((nativeW > nativeH) ? nativeW : nativeH);
					float drawW = nativeW * scale;
					float drawH = nativeH * scale;
					float iconY = bottomToolbarStart.y + (BOTTOM_TOOLBAR_HEIGHT - drawH) * 0.5f;
					iconMin = ImVec2(cursorX, iconY);
					iconMax = ImVec2(iconMin.x + drawW, iconMin.y + drawH);
					iconDrawW = drawW;
					iconDrawH = drawH;
					hasIcon = true;
					drawlist->AddImage(
						static_cast<ImTextureID>(reinterpret_cast<intptr_t>(tex)),
						iconMin,
						iconMax,
						ImVec2(0, 0), ImVec2(1, 1),
						IM_COL32(255, 255, 255, 255));
					cursorX += drawW + 8.0f;
				}
			}
		}

		std::string logMessage = lastEntry.message;
		ImVec2 textSize = ImGui::CalcTextSize(logMessage.c_str());
		ImVec2 textPos = {
			cursorX,
			bottomToolbarStart.y + (BOTTOM_TOOLBAR_HEIGHT - textSize.y) * 0.5f
		};
		ImGui::SetCursorScreenPos(textPos);
		ImGui::Text("%s", logMessage.c_str());

		// Click detection around icon + text
		{
			float rectLeft = bottomToolbarStart.x + padding.x;
			float rectRight = cursorX + textSize.x;
			float rectTop;
			float rectBottom;
			if (hasIcon)
			{
				rectTop = iconMin.y < textPos.y ? iconMin.y : textPos.y;
				rectBottom = iconMax.y > textPos.y + textSize.y ? iconMax.y : textPos.y + textSize.y;
			}
			else
			{
				rectTop = textPos.y;
				rectBottom = textPos.y + textSize.y;
			}
			// Slight padding around hit area for easier clicking
			const float hitPadding = 4.0f;
			ImVec2 rectMin = ImVec2(rectLeft - hitPadding, rectTop - hitPadding);
			ImVec2 rectMax = ImVec2(rectRight + hitPadding, rectBottom + hitPadding);

			if (!logMessage.empty() || hasIcon)
			{
				if (ImGui::IsMouseHoveringRect(rectMin, rectMax))
				{
					ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						GetWorkspace().ToggleLogHistoryPanelOpen();
					}
				}
			}
		}

		if (GetWorkspace().IsLogHistoryPanelOpen())
		{
			ImVec2 panelMin = ImVec2(bottomToolbarStart.x, bottomToolbarStart.y - LOG_PANEL_HEIGHT);
			ImVec2 panelSize = ImVec2(bottomToolbarEnd.x - bottomToolbarStart.x, LOG_PANEL_HEIGHT);
			ImGui::SetNextWindowPos(panelMin);
			ImGui::SetNextWindowSize(panelSize);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(33, 38, 52, 255));
			ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(60, 65, 80, 255));

			ImGuiWindowFlags overlayFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking;

			ImDrawList* fgdrawlist = ImGui::GetForegroundDrawList();

			ImVec2 topPos = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + TOP_TOOLBAR_HEIGHT };
			fgdrawlist->AddRectFilled(
				topPos,
				{ topPos.x + ImGui::GetWindowSize().x, topPos.y + (ImGui::GetWindowSize().y - TOP_TOOLBAR_HEIGHT) - (BOTTOM_TOOLBAR_HEIGHT + LOG_PANEL_HEIGHT) },
				IM_COL32(0, 0, 0, 100)
			);

			if (ImGui::Begin(FormatId("", WINDOW_ID, "LOG_PANEL").c_str(), nullptr, overlayFlags))
			{
				if (ImGui::BeginChild(
					FormatId("", CHILD_ID, "LOG_PANEL").c_str(),
					ImVec2(
						ImGui::GetContentRegionAvail().x,
						ImGui::GetContentRegionAvail().y
					),
					ImGuiChildFlags_Borders
				))
				{
					const float entryIconSize = 16.0f;
					if (m_aLogHistory.empty())
					{
						ImGui::TextUnformatted("No log messages yet.");
					}
					else
					{
						size_t i = 0;
						for (const LogEntry& entry : m_aLogHistory)
						{
							ImVec2 startPos = ImGui::GetCursorPos();
							ImGui::Selectable(FormatId("", SELECTABLE_ID, std::to_string(i), "LOG_ENTRY").c_str());

							ImGui::SetCursorPos(startPos);
							ID3D11ShaderResourceView* tex = entry.icon.empty() ? nullptr : dx11::SVGTextureCache::Get(entry.icon);
							if (tex)
							{
								int nativeW = dx11::SVGTextureCache::GetWidth(entry.icon);
								int nativeH = dx11::SVGTextureCache::GetHeight(entry.icon);
								if (nativeW > 0 && nativeH > 0)
								{
									float scale = entryIconSize / static_cast<float>(nativeW > nativeH ? nativeW : nativeH);
									float drawW = nativeW * scale;
									float drawH = nativeH * scale;
									ImVec2 cursor = ImGui::GetCursorScreenPos();
									float textH = ImGui::GetTextLineHeight();
									float iconY = cursor.y + (textH - drawH) * 0.5f;
									ImVec2 iconMinE = ImVec2(cursor.x, iconY);
									ImVec2 iconMaxE = ImVec2(iconMinE.x + drawW, iconMinE.y + drawH);
									ImGui::GetWindowDrawList()->AddImage(
										static_cast<ImTextureID>(reinterpret_cast<intptr_t>(tex)),
										iconMinE, iconMaxE);
									ImGui::SetCursorScreenPos(ImVec2(cursor.x + drawW + 6.0f, cursor.y));
								}
							}
							ImGui::TextUnformatted(entry.message.c_str());
							i++;
						}
						if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
						{
							ImGui::SetScrollHereY(1.0f);
						}
					}
				}
				ImGui::EndChild();
			}
			ImGui::End();
			ImGui::PopStyleColor(2);
			ImGui::PopStyleVar();
		}
	}

	//---------------------------------------------------------------------
	bool BottomToolbarWindow::OnInitialized()
	{
		logger::GetLogEvent() += std::bind(&BottomToolbarWindow::OnLogEvent, this, std::placeholders::_1, std::placeholders::_2);
		return true;
	}

	//---------------------------------------------------------------------
	void BottomToolbarWindow::OnLogEvent(core::LogLevel a_Level, const std::string& a_sMessage)
	{
		std::string icon;
		switch (a_Level)
		{
			case core::LogLevel::Success:
			{
				icon = "icon_checkmark.svg";
				break;
			}
			case core::LogLevel::Error:
			{
				icon = "icon_error.svg";
				break;
			}
			case core::LogLevel::Warning:
			{
				icon = "icon_warning.svg";
				break;
			}
			case core::LogLevel::Info:
			{
				icon = "icon_info.svg";
				break;
			}
		}
		
		LogEntry entry;
		entry.level = a_Level;
		entry.icon = icon;
		entry.message = a_sMessage;
		m_aLogHistory.push_back(std::move(entry));
		if (m_aLogHistory.size() > LOG_MAX_HISTORY)
		{
			m_aLogHistory.erase(m_aLogHistory.begin());
		}
	}
}
