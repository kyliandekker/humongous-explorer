#include "imgui/imgui.h"
#include "imgui/implot.h"

#include "helib/core/Log.h"

#include "win32/Window.h"
#include "dx11/DX11System.h"
#include "dx11/SVGTextureCache.h"
#include "imgui/ImGuiSystem.h"
#include "editor/Workspace.h"

#include "ui/AppLogger.h"

#include "win32/winfile.h"
#include "helib/file/file.h"

#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static void HelibLogCallback(humongousexplorer::core::LogLevel a_Level, const std::string& a_sMessage)
{
	humongousexplorer::logger::GetLogEvent().invoke(a_Level, a_sMessage);

	const char* prefix = "";
	switch (a_Level)
	{
		case humongousexplorer::core::LogLevel::Success: prefix = "[OK]     "; break;
		case humongousexplorer::core::LogLevel::Error:   prefix = "[FAIL]   "; break;
		case humongousexplorer::core::LogLevel::Warning: prefix = "[WARN]   "; break;
		case humongousexplorer::core::LogLevel::Info:    prefix = "[INFO]   "; break;
	}
	printf("%s%s\n", prefix, a_sMessage.c_str());
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	humongousexplorer::core::InitializeLog();
	humongousexplorer::core::SetLogCallback(HelibLogCallback);

	ImGui_ImplWin32_EnableDpiAwareness();
	float mainScale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

	humongousexplorer::win32::Window& window = humongousexplorer::win32::GetWin32Window();
	std::string appName = humongousexplorer::GetWorkspace().GetAppName();
	std::wstring wAppName(appName.begin(), appName.end());
	if (!window.Initialize(hInstance, (int)(1280 * mainScale), (int)(800 * mainScale), wAppName.c_str(), true))
	{
		return 1;
	}
	window.SetMessageHook(&ImGui_ImplWin32_WndProcHandler);
	HWND hwnd = window.GetHandle();

	if (!humongousexplorer::dx11::GetDX11System().Initialize(hwnd))
	{
		humongousexplorer::dx11::GetDX11System().Destroy();
		window.Destroy();
		return 1;
	}

	window.Show(SW_SHOWDEFAULT);

	ImColor clearColor = IM_COL32(21, 26, 36, 255);

	humongousexplorer::GetWorkspace().Initialize();

	fs::path appDataPath = humongousexplorer::file::GetAppDataPath().string() + "/humongousexplorer";
	humongousexplorer::file::CreateFolder(appDataPath);

	humongousexplorer::imgui::GetImGuiSystem().SetIniPath(appDataPath.string());
	humongousexplorer::imgui::GetImGuiSystem().Initialize();

	bool running = true;
	while (running)
	{
		running = window.ProcessMessages();
		if (!running)
		{
			break;
		}

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

	humongousexplorer::core::DestroyLog();

	return 0;
}