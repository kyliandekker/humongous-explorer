#pragma once

#include "imgui/windows/MainWindowDock.h"

namespace humongousexplorer::imgui
{
	class HEBaseWindow : public BaseWindow
	{
	public:
		/// <summary>
		/// Constructs a window.
		/// </summary>
		/// <param name="a_Flags">The flags used for disabling or enabling certain behaviours in the window.</param>
		/// <param name="a_sName">The name of the window (displayed at the top).</param>
		/// <param name="a_sWindowID">The ID of the window.</param>
		/// <param name="a_bFullScreen">Whether the window is full screen or not.</param>
		HEBaseWindow(ImGuiWindowFlags a_Flags, const std::string& a_sName, std::string a_sWindowID, bool a_bFullScreen = false) : BaseWindow(a_Flags, a_sName, a_sWindowID, a_bFullScreen)
		{}

		void Render() override;
	};

	class LoggerDependentWindow : public HEBaseWindow
	{
	public:
		/// <summary>
		/// Constructs a window.
		/// </summary>
		/// <param name="a_Flags">The flags used for disabling or enabling certain behaviours in the window.</param>
		/// <param name="a_sName">The name of the window (displayed at the top).</param>
		/// <param name="a_sWindowID">The ID of the window.</param>
		/// <param name="a_bFullScreen">Whether the window is full screen or not.</param>
		LoggerDependentWindow(ImGuiWindowFlags a_Flags, const std::string& a_sName, std::string a_sWindowID, bool a_bFullScreen = false) : HEBaseWindow(a_Flags, a_sName, a_sWindowID, a_bFullScreen)
		{}

		void Render() override;
	};
}