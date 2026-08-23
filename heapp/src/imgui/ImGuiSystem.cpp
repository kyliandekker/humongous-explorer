#include "ImGuiSystem.h"

// external
#include <imgui/backends/imgui_impl_win32.h>
#include <imgui/backends/imgui_impl_dx11.h>
#include <imgui/imgui_internal.h>
#include <imgui/font_arial.h>
#include <imgui/font_icon.h>
#include <imgui/windows/BaseWindow.h>
#include <implot.h>

// graphics
#include "dx11/DX11System.h"

#include "win32/Window.h"

#include "imgui/Theme.h"
#include "imgui/windows/ArchiveContentsWindow.h"
#include "imgui/windows/EditorWindowDock.h"
#include "imgui/windows/RoomContentWindow.h"
#include "imgui/windows/PreviewWindow.h"
#include "imgui/windows/InfoPanelWindow.h"
#include "imgui/windows/TopToolbarWindow.h"
#include "imgui/windows/BottomToolbarWindow.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	// ImGuiSystem
	//---------------------------------------------------------------------
	ImGuiSystem& GetImGuiSystem()
	{
		static ImGuiSystem system;
		return system;
	}

	//---------------------------------------------------------------------
	ImGuiSystem::ImGuiSystem()
	{}

	//---------------------------------------------------------------------
	bool ImGuiSystem::Initialize()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = m_sIniPath.c_str();

		if (!CreateContextWin32() || !CreateContextDX11())
		{
			return false;
		}
		ImPlot::CreateContext();

		CreateImGui();

		win32::Window& window = win32::GetWin32Window();
		RECT rc;
		GetClientRect(window.GetHandle(), &rc);
		Resize(rc.right - rc.left, rc.bottom - rc.top);

		m_aWindows.emplace_back(std::make_unique<EditorWindowDock>());
		m_aWindows.emplace_back(std::make_unique<TopToolbarWindow>());
		m_aWindows.emplace_back(std::make_unique<ArchiveContentsWindow>());
		m_aWindows.emplace_back(std::make_unique<RoomContentWindow>());
		m_aWindows.emplace_back(std::make_unique<PreviewWindow>());
		m_aWindows.emplace_back(std::make_unique<InfoPanelWindow>());
		m_aWindows.emplace_back(std::make_unique<BottomToolbarWindow>());
		InitializeWindows();

		return true;
	}

	//---------------------------------------------------------------------
	bool ImGuiSystem::InitializeWindows()
	{
		for (std::unique_ptr<BaseWindow>& window : m_aWindows)
		{
			window->Initialize();
		}

		return true;
	}

	//---------------------------------------------------------------------
	bool ImGuiSystem::Destroy()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		return true;
	}

	//---------------------------------------------------------------------
	bool ImGuiSystem::CreateContextWin32()
	{
		win32::Window& window = win32::GetWin32Window();
		if (!ImGui_ImplWin32_Init(window.GetHandle()))
		{
			return false;
		}

		return true;
	}

	//---------------------------------------------------------------------
	bool ImGuiSystem::CreateContextDX11()
	{
		dx11::DX11System& dx11System = dx11::GetDX11System();

		if (!ImGui_ImplDX11_Init(dx11System.GetDevice(), dx11System.GetDeviceContext()))
		{
			return true;
		}

		return true;
	}

	//---------------------------------------------------------------------
	void ImGuiSystem::CreateImGui()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

		(void) io;

		win32::Window& window = win32::GetWin32Window();

		ImGui_ImplWin32_EnableDpiAwareness();
		float mainScale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

		m_fFontSize = 15.0f;

		// setup Dear ImGui style
		ImGui::StyleColorsDark();

		ImFontConfig font_config_bold;
		font_config_bold.FontDataOwnedByAtlas = false;
		m_pBoldFont = io.Fonts->AddFontFromMemoryTTF(&font::arialBold, sizeof(font::arialBold), m_fFontSize, &font_config_bold);

		ImFontConfig font_config_default;
		font_config_default.FontDataOwnedByAtlas = false;
		m_pDefaultFont = io.Fonts->AddFontFromMemoryTTF(&font::arial, sizeof(font::arial), m_fFontSize, &font_config_default);

		constexpr ImWchar icons_ranges_b[] = { icon::FONT_START, icon::FONT_END, 0 };
		ImFontConfig icons_config_m;
		icons_config_m.MergeMode = true;
		icons_config_m.PixelSnapH = true;
		icons_config_m.FontDataOwnedByAtlas = false;
		m_pIconFont = io.Fonts->AddFontFromMemoryTTF(&icon::ICON, sizeof(icon::ICON), m_fFontSize, &icons_config_m, icons_ranges_b);

		io.Fonts->Build();

		ApplyTheme();

		ImGuiStyle& style = ImGui::GetStyle();
		style.ScaleAllSizes(mainScale);
		style.FontScaleDpi = mainScale;
	}

	//---------------------------------------------------------------------
	void ImGuiSystem::Resize(uint32_t a_iWidth, uint32_t a_iHeight)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize.x = static_cast<float>(a_iWidth);
		io.DisplaySize.y = static_cast<float>(a_iHeight);
	}

	//---------------------------------------------------------------------
	void ImGuiSystem::Render()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		const ImGuiIO& io = ImGui::GetIO();

		for (std::unique_ptr<BaseWindow>& window : m_aWindows)
		{
			window->Render();
		}

		ImGui::EndFrame();
		ImGui::Render();

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();

			// Pump Win32 messages for ImGui secondary viewport windows that were just
			// created/updated by UpdatePlatformWindows(), so that window state (size,
			// position, visibility) is synchronized before the first render.
			MSG msg = {};
			while (::PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessageW(&msg);
			}

			ImGui::RenderPlatformWindowsDefault();
		}
	}

	//---------------------------------------------------------------------
	void ImGuiSystem::UpdateMouseCursor()
	{
		// Let Win32 own cursor on resize borders (same bw as Window::WM_NCHITTEST)
		{
			HWND hwnd = nullptr;
			if (ImGui::GetMainViewport())
			{
				hwnd = (HWND)ImGui::GetMainViewport()->PlatformHandleRaw;
			}
			if (!hwnd)
			{
				hwnd = ::GetForegroundWindow();
			}
			if (hwnd && !::IsZoomed(hwnd))
			{
				POINT p; ::GetCursorPos(&p);
				RECT rc; ::GetWindowRect(hwnd, &rc);
				const int bx = ::GetSystemMetrics(SM_CXFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER);
				const int by = ::GetSystemMetrics(SM_CYFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER);
				const int bw = max(max(bx, by) + 4, 12);
				const bool onBorder = (p.x < rc.left + bw) || (p.x >= rc.right - bw) || (p.y < rc.top + bw) || (p.y >= rc.bottom - bw);
				if (onBorder)
				{
					return;
				}
			}
		}

		bool s_IsHovering = ImGui::IsAnyItemHovered();
		if (!s_IsHovering)
		{
			return;
		}

		if (s_IsHovering && ImGui::GetMouseCursor() == ImGuiMouseCursor_Arrow)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}

		LPTSTR win32_cursor = IDC_ARROW;
		ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
		switch (imgui_cursor)
		{
			case ImGuiMouseCursor_TextInput:
			{
				win32_cursor = IDC_IBEAM;
				break;
			}
			case ImGuiMouseCursor_ResizeAll:
			{
				win32_cursor = IDC_SIZEALL;
				break;
			}
			case ImGuiMouseCursor_ResizeNS:
			{
				win32_cursor = IDC_SIZENS;
				break;
			}
			case ImGuiMouseCursor_ResizeEW:
			{
				win32_cursor = IDC_SIZEWE;
				break;
			}
			case ImGuiMouseCursor_ResizeNESW:
			{
				win32_cursor = IDC_SIZENESW;
				break;
			}
			case ImGuiMouseCursor_ResizeNWSE:
			{
				win32_cursor = IDC_SIZENWSE;
				break;
			}
			case ImGuiMouseCursor_Hand:
			{
				win32_cursor = IDC_HAND;
				break;
			}
			case ImGuiMouseCursor_NotAllowed:
			{
				win32_cursor = IDC_NO;
				break;
			}
			default:
			{
				win32_cursor = IDC_ARROW;
				break;
			}
		}

		// Set the system cursor using Win32 API
		::SetCursor(LoadCursor(NULL, win32_cursor));
	}

	//---------------------------------------------------------------------
	void ImGuiSystem::SetIniPath(const std::string& a_sPath)
	{
		m_sIniPath = a_sPath + "/imgui.ini";
	}

	//---------------------------------------------------------------------
	void ImGuiSystem::SetDroppedFile(const std::string& a_sPath, ImVec2 a_vDropPos)
	{
		s_sDroppedFile = a_sPath;
		s_vDroppedFilePos = a_vDropPos;
	}

	//---------------------------------------------------------------------
	std::string ImGuiSystem::ConsumeDroppedFile()
	{
		std::string path = std::move(s_sDroppedFile);
		s_sDroppedFile.clear();
		return path;
	}

	//---------------------------------------------------------------------
	ImVec2 ImGuiSystem::GetDroppedFilePosition()
	{
		ImVec2 pos = s_vDroppedFilePos;
		s_vDroppedFilePos = {};
		return pos;
	}

	//---------------------------------------------------------------------
	LRESULT ImGuiSystem::WndProcHandler(HWND a_hWnd, UINT a_iMsg, WPARAM a_wParam, LPARAM a_lParam)
	{
		return ImGui_ImplWin32_WndProcHandler(a_hWnd, a_iMsg, a_wParam, a_lParam);
	}

	//---------------------------------------------------------------------
	ImFont* ImGuiSystem::GetBoldFont()
	{
		return m_pBoldFont;
	}

	//---------------------------------------------------------------------
	ImFont* ImGuiSystem::GetDefaultFont()
	{
		return m_pDefaultFont;
	}
}