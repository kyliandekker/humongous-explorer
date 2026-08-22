#pragma once

// external
#include <imgui/imgui.h>
#include <wtypes.h>
#include <memory>
#include <string>
#include <vector>

namespace humongousexplorer::dx11
{ 
	class DX11System; 
}
namespace humongousexplorer::imgui
{
	class ImGuiSystem;
	extern ImGuiSystem& GetImGuiSystem();

	enum ImGuiExtraCol_
	{
		ImGuiExtraCol_Accent,
		ImGuiExtraCol_AccentHovered,
		ImGuiExtraCol_AccentActive,
		ImGuiExtraCol_TabInactive,
		ImGuiExtraCol_COUNT
	};
	inline ImVec4 ExtraColors[ImGuiExtraCol_COUNT];

	class BaseWindow;
	class BottomToolbar;

	//---------------------------------------------------------------------
	// ImGuiSystem
	//---------------------------------------------------------------------
	class ImGuiSystem
	{
	public:
		ImGuiSystem();

		/// <summary>
		/// Initializes the imgui system.
		/// </summary>
		bool Initialize();

		/// <summary>
		/// Initializes the windows.
		/// </summary>
		bool InitializeWindows();

		/// <summary>
		/// Destroys all imgui assets.
		/// </summary>
		bool Destroy();

		/// <summary>
		/// Updates ImGui display size to match the new window dimensions.
		/// </summary>
		/// <param name="a_iWidth">The new client area width.</param>
		/// <param name="a_iHeight">The new client area height.</param>
		void Resize(uint32_t a_iWidth, uint32_t a_iHeight);

		/// <summary>
		/// Renders the ImGui frame.
		/// </summary>
		void Render();
	private:
		/// <summary>
		/// Handles Windows messages for the editor's window.
		/// </summary>
		/// <param name="a_hWnd">Handle to the window.</param>
		/// <param name="a_iMsg">Message identifier.</param>
		/// <param name="a_wParam">Additional message information (WPARAM).</param>
		/// <param name="a_lParam">Additional message information (LPARAM).</param>
		/// <returns>The result of the message processing.</returns>
		LRESULT CALLBACK WndProcHandler(HWND a_hWnd, UINT a_iMsg, WPARAM a_wParam, LPARAM a_lParam);

		/// <summary>
		/// Creates the ImGui context for Win32.
		/// </summary>
		/// <returns>True if the context creation succeeds, otherwise false.</returns>
		bool CreateContextWin32();

		/// <summary>
		/// Creates the ImGui context for DirectX 11.
		/// </summary>
		/// <returns>True if the context creation succeeds, otherwise false.</returns>
		bool CreateContextDX11();

		/// <summary>
		/// Initializes the ImGui UI components.
		/// </summary>
		void CreateImGui();
	public:
		/// <summary>
		/// Updates the mouse cursor when hovering, clicking, etc.
		/// </summary>
		void UpdateMouseCursor();

		/// <summary>
		/// Retrieves the bold font.
		/// </summary>
		/// <returns>A pointer to the ImFont.</returns>
		ImFont* GetBoldFont();
		ImFont* GetDefaultFont();

		void SetIniPath(const std::string& a_sPath);

		void SetDroppedFile(const std::string& a_sPath, ImVec2 a_vDropPos);
		std::string ConsumeDroppedFile();
		ImVec2 GetDroppedFilePosition();
	private:
		std::string s_sDroppedFile;
		ImVec2 s_vDroppedFilePos = {};

		ImFont* m_pDefaultFont = nullptr;
		ImFont* m_pBoldFont = nullptr;
		ImFont* m_pIconFont = nullptr;

		float m_fFontSize = 0.0f; /// Default font size for ImGui.

		std::vector<std::unique_ptr<BaseWindow>> m_aWindows;

		std::string m_sIniPath;

		friend humongousexplorer::dx11::DX11System;
	};
}