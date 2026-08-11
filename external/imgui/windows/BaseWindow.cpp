#include "BaseWindow.h"

// external
#include <imgui.h>

#include "Helpers.h"

namespace humongousexplorer::imgui
{
	ImFont* GetBoldFont();
	ImFont* GetDefaultFont();
}

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	// BaseWindow
	//---------------------------------------------------------------------
	BaseWindow::BaseWindow(ImGuiWindowFlags a_Flags, const std::string& a_sName, std::string a_sWindowID, bool a_bFullScreen) : m_Flags(a_Flags), m_sName(a_sName), m_sWindowID(a_sWindowID), m_bFullScreen(a_bFullScreen), ImGuiUIView()
	{}

	//---------------------------------------------------------------------
	BaseWindow::~BaseWindow() = default;

	//---------------------------------------------------------------------
	bool BaseWindow::Destroy()
	{
		return true;
	}

	//---------------------------------------------------------------------
	bool BaseWindow::WindowBegin()
	{
		m_bEnabled = true;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetFontSize(), ImGui::GetFontSize()));
		if (m_bHideCloseButton)
		{
			bool open = ImGui::Begin(FormatId(m_sName, WINDOW_ID, m_sWindowID).c_str(), nullptr, m_Flags);
			return open;
		}
		else
		{
			bool open = ImGui::Begin(FormatId(m_sName, WINDOW_ID, m_sWindowID).c_str(), &m_bEnabled, m_Flags);
			return open;
		}
	}

	//---------------------------------------------------------------------
	void BaseWindow::WindowEnd()
	{
		ImDrawList* pDrawList = ImGui::GetForegroundDrawList();
		ImVec2 windowMin = ImGui::GetWindowPos();
		ImVec2 windowMax = {
			windowMin.x + ImGui::GetWindowSize().x,
			windowMin.y + ImGui::GetWindowSize().y,
		};

		ImGui::End();
		ImGui::PopStyleVar();
	}

	//---------------------------------------------------------------------
	void BaseWindow::Render()
	{
		if (!m_bVisible)
		{
			return;
		}

		if (m_bFullScreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();

			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
		}

		m_bBegin = false;
		if (!m_bRenderWindowItself)
		{
			m_bBegin = WindowBegin();
		}

		if (m_bFocusMyWindow)
		{
			ImGui::SetWindowFocus(); // focuses the current window
			m_bFocusMyWindow = false;
		}

		if (m_bBegin && m_bInitialized)
		{
			Update();
		}

		if (!m_bRenderWindowItself)
		{
			WindowEnd();
		}
	}

	//---------------------------------------------------------------------
	void BaseWindow::SetSize(ImVec2 a_vSize)
	{
		m_vSize = a_vSize;
	}

	//---------------------------------------------------------------------
	void BaseWindow::Initialize()
	{
		if (m_bInitialized)
		{
			return;
		}

		if (!OnInitialized())
		{
			return;
		}
		m_bInitialized = true;
	}

	//---------------------------------------------------------------------
	bool BaseWindow::IsFullScreen() const
	{
		return m_bFullScreen;
	}
}