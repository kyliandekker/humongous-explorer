#include "imgui/imgui.h"
#include "imgui/implot.h"

#include "core/Log.h"
#include "logger/Logger.h"

#include "win32/Window.h"
#include "dx11/DX11System.h"
#include "dx11/SVGTextureCache.h"
#include "imgui/ImGuiSystem.h"
#include "editor/Workspace.h"

#include "win32/winfile.h"

#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static void HelibLogCallback(humongousexplorer::core::LogLevel a_Level, const std::string& a_sMessage)
{
	humongousexplorer::LogSeverity severity;
	switch (a_Level)
	{
		case humongousexplorer::core::LogLevel::Success: severity = humongousexplorer::LOGSEVERITY_SUCCESS; break;
		case humongousexplorer::core::LogLevel::Error:   severity = humongousexplorer::LOGSEVERITY_ERROR; break;
		case humongousexplorer::core::LogLevel::Warning: severity = humongousexplorer::LOGSEVERITY_WARNING; break;
		case humongousexplorer::core::LogLevel::Info:    severity = humongousexplorer::LOGSEVERITY_INFO; break;
		default: severity = humongousexplorer::LOGSEVERITY_INFO; break;
	}
	humongousexplorer::GetLogger().Log(severity, a_sMessage, __FILE__, __LINE__);
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	humongousexplorer::core::InitializeLog();
	humongousexplorer::GetLogger().Initialize(true);
	humongousexplorer::core::SetLogCallback(HelibLogCallback);

	ImGui_ImplWin32_EnableDpiAwareness();
	float mainScale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

	humongousexplorer::win32::Window& window = humongousexplorer::win32::GetWin32Window();
	if (!window.Initialize(hInstance, (int)(1280 * mainScale), (int)(800 * mainScale), L"Humongous Explorer", true))
	{
		humongousexplorer::GetLogger().Destroy();
		return 1;
	}
	window.SetMessageHook(&ImGui_ImplWin32_WndProcHandler);
	HWND hwnd = window.GetHandle();

	if (!humongousexplorer::dx11::GetDX11System().Initialize(hwnd))
	{
		humongousexplorer::dx11::GetDX11System().Destroy();
		window.Destroy();
		humongousexplorer::GetLogger().Destroy();
		return 1;
	}

	window.Show(SW_SHOWDEFAULT);

	ImColor clearColor = IM_COL32(21, 26, 36, 255);

	humongousexplorer::GetWorkspace().Initialize();

	fs::path appDataPath = humongousexplorer::file::GetAppDataPath().string() + "/humongousexplorer";
	humongousexplorer::file::CreateDirectory(appDataPath);

	humongousexplorer::imgui::GetImGuiSystem().SetIniPath(appDataPath.string());
	humongousexplorer::imgui::GetImGuiSystem().Initialize();

	bool running = true;
	while (running)
	{
		running = window.ProcessMessages();
		if (!running)
			break;

		if (window.HasPendingResize())
		{
			uint32_t width = 0, height = 0;
			window.ConsumeResize(width, height);
			humongousexplorer::dx11::GetDX11System().Resize(width, height);
		}

		if (window.HasDroppedFile())
		{
			std::string path = window.ConsumeDroppedFile();
			POINT pt = window.GetDroppedFilePosition();
			humongousexplorer::imgui::GetImGuiSystem().SetDroppedFile(path, ImVec2(static_cast<float>(pt.x), static_cast<float>(pt.y)));
		}
		const float clear_color_with_alpha[4] = { clearColor.Value.x * clearColor.Value.w, clearColor.Value.y * clearColor.Value.w, clearColor.Value.z * clearColor.Value.w, clearColor.Value.w };
		humongousexplorer::dx11::GetDX11System().BeginFrame(clear_color_with_alpha);

		humongousexplorer::imgui::GetImGuiSystem().Render();

		humongousexplorer::imgui::GetImGuiSystem().UpdateMouseCursor();

		humongousexplorer::dx11::GetDX11System().EndFrame(1);
	}

	humongousexplorer::imgui::GetImGuiSystem().Destroy();

	humongousexplorer::dx11::SVGTextureCache::Shutdown();
	humongousexplorer::dx11::GetDX11System().Destroy();
	window.Destroy();

	humongousexplorer::GetLogger().Destroy();
	humongousexplorer::core::DestroyLog();

	return 0;
}