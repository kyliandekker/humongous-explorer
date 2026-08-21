#include "imgui/imgui.h"
#include "imgui/implot.h"

#include "core/Log.h"
#include "logger/Logger.h"

#include "win32/Window.h"
#include "dx11/DX11System.h"
#include "dx11/SVGTextureCache.h"
#include "imgui/ImGuiSetup.h"
#include "editor/Workspace.h"

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

	humongousexplorer::win32::Window window;
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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void) io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	 // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	  // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		 // Enable Docking
	io.IniFilename = nullptr;								 // Disable imgui.ini (use DockBuilder layout)

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(mainScale);
	style.FontScaleDpi = mainScale;

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(humongousexplorer::dx11::GetDX11System().GetDevice(), humongousexplorer::dx11::GetDX11System().GetDeviceContext());

	ImColor clearColor = IM_COL32(21, 26, 36, 255);

	humongousexplorer::GetWorkspace().Initialize();
	humongousexplorer::imgui::Initialize();

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
			humongousexplorer::imgui::SetDroppedFile(path, ImVec2(static_cast<float>(pt.x), static_cast<float>(pt.y)));
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		humongousexplorer::imgui::Render();

		humongousexplorer::imgui::UpdateMouseCursor();

		ImGui::Render();
		const float clear_color_with_alpha[4] = { clearColor.Value.x * clearColor.Value.w, clearColor.Value.y * clearColor.Value.w, clearColor.Value.z * clearColor.Value.w, clearColor.Value.w };
		humongousexplorer::dx11::GetDX11System().BeginFrame(clear_color_with_alpha);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		humongousexplorer::dx11::GetDX11System().EndFrame(1);
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();

	humongousexplorer::dx11::SVGTextureCache::Shutdown();
	humongousexplorer::dx11::GetDX11System().Destroy();
	window.Destroy();

	humongousexplorer::GetLogger().Destroy();
	humongousexplorer::core::DestroyLog();

	return 0;
}