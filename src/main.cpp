#include "imgui/imgui.h"

#include "logger/Logger.h"

#include "win32/Window.h"
#include "dx11/DX11System.h"
#include "dx11/SVGTextureCache.h"
#include "imgui/ImGuiSetup.h"
#include "editor/Workspace.h"

#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    humongousexplorer::GetLogger().Initialize(true);

    ImGui_ImplWin32_EnableDpiAwareness();
    float mainScale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    humongousexplorer::win32::Window window;
    if (!window.Initialize(hInstance, (int)(1280 * mainScale), (int)(800 * mainScale), L"Humongous Explorer"))
    {
        humongousexplorer::GetLogger().Destroy();
        return 1;
    }
    window.SetMessageHook(&ImGui_ImplWin32_WndProcHandler);
    HWND hwnd = window.GetHandle();

    if (!humongousexplorer::GetDX11System().Initialize(hwnd))
    {
        humongousexplorer::GetDX11System().Destroy();
        window.Destroy();
        humongousexplorer::GetLogger().Destroy();
        return 1;
    }

    window.Show(SW_SHOWDEFAULT);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.IniFilename = nullptr;                                 // Disable imgui.ini (use DockBuilder layout)

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(mainScale);
    style.FontScaleDpi = mainScale;

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(humongousexplorer::GetDX11System().GetDevice(), humongousexplorer::GetDX11System().GetDeviceContext());

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
            humongousexplorer::GetDX11System().Resize(width, height);
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        humongousexplorer::imgui::Render();

        ImGui::Render();
        const float clear_color_with_alpha[4] = { clearColor.Value.x * clearColor.Value.w, clearColor.Value.y * clearColor.Value.w, clearColor.Value.z * clearColor.Value.w, clearColor.Value.w };
        humongousexplorer::GetDX11System().BeginFrame(clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        humongousexplorer::GetDX11System().EndFrame(1);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    humongousexplorer::dx11::SVGTextureCache::Shutdown();
    humongousexplorer::GetDX11System().Destroy();
    window.Destroy();

    humongousexplorer::GetLogger().Destroy();

    return 0;
}