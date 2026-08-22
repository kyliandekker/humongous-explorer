#include "Window.h"

// external includes
#include <format>

namespace humongousexplorer::win32
{
	//---------------------------------------------------------------------
	Window& GetWin32Window()
	{
		static Window window;
		return window;
	}

	//---------------------------------------------------------------------
	// Window
	//---------------------------------------------------------------------
	Window::~Window()
	{
		Destroy();
	}

	//---------------------------------------------------------------------
	bool Window::Initialize(HINSTANCE a_HInstance, int a_iWidth, int a_iHeight, const wchar_t* a_sTitle, bool a_bFrameless)
	{
		m_HInstance = a_HInstance;
		m_bFrameless = a_bFrameless;

		m_wsClassName = std::format(L"{}_%08X", a_sTitle, GetCurrentProcessId());

		// WS_THICKFRAME is required for resizability.
		// WS_MAXIMIZEBOX / WS_MINIMIZEBOX are needed for Aero snap + maximize via taskbar / Win+Arrow.
		DWORD style = a_bFrameless
			? (WS_POPUP | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_VISIBLE)
			: WS_OVERLAPPEDWINDOW;

		WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, &Window::WndProc, 0L, 0L, a_HInstance, nullptr, ::LoadCursorW(nullptr, reinterpret_cast<LPCWSTR>(IDC_ARROW)), nullptr, nullptr, m_wsClassName.c_str(), nullptr };

		if (!::RegisterClassExW(&wc))
		{
			DWORD err = ::GetLastError();
			WNDCLASSEXW existing;
			if (err != ERROR_CLASS_ALREADY_EXISTS || ::GetClassInfoExW(a_HInstance, m_wsClassName.c_str(), &existing) == 0)
			{
				return false;
			}
		}

		m_HWnd = ::CreateWindowW(m_wsClassName.c_str(), a_sTitle, style, 100, 100, a_iWidth, a_iHeight, nullptr, nullptr, a_HInstance, this);
		if (!m_HWnd)
		{
			::UnregisterClassW(m_wsClassName.c_str(), a_HInstance);
			return false;
		}

		::DragAcceptFiles(m_HWnd, TRUE);

		if (m_bFrameless)
			ApplyRoundedCorners();

		return true;
	}

	//---------------------------------------------------------------------
	void Window::Destroy()
	{
		if (m_HWnd)
		{
			::DestroyWindow(m_HWnd);
			m_HWnd = nullptr;
		}
		if (!m_wsClassName.empty() && m_HInstance)
		{
			::UnregisterClassW(m_wsClassName.c_str(), m_HInstance);
		}
	}

	//---------------------------------------------------------------------
	bool Window::ProcessMessages()
	{
		MSG msg;
		bool running = true;
		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			switch (msg.message)
			{
				case WM_QUIT:
				{
					running = false;
					break;
				}
			}
		}
		return running;
	}

	//---------------------------------------------------------------------
	void Window::Show(int a_iShowCmd)
	{
		if (!m_HWnd)
		{
			return;
		}
		::ShowWindow(m_HWnd, a_iShowCmd);
		::UpdateWindow(m_HWnd);
	}

	//---------------------------------------------------------------------
	void Window::SetMessageHook(MessageHook a_Hook)
	{
		m_MessageHook = a_Hook;
	}

	//---------------------------------------------------------------------
	HWND Window::GetHandle() const
	{
		return m_HWnd;
	}

	//---------------------------------------------------------------------
	void Window::StartDrag()
	{
		POINT cursor;
		GetCursorPos(&cursor);
		RECT rc;
		GetWindowRect(m_HWnd, &rc);
		m_iDragOffset.x = cursor.x - rc.left;
		m_iDragOffset.y = cursor.y - rc.top;
		m_bDragging = true;
		SetCapture(m_HWnd);
	}

	//---------------------------------------------------------------------
	void Window::Minimize()
	{
		ShowWindow(m_HWnd, SW_MINIMIZE);
	}

	//---------------------------------------------------------------------
	void Window::Maximize()
	{
		ShowWindow(m_HWnd, IsZoomed(m_HWnd) ? SW_RESTORE : SW_MAXIMIZE);
	}

	//---------------------------------------------------------------------
	void Window::Close()
	{
		PostMessage(m_HWnd, WM_CLOSE, 0, 0);
	}

	//---------------------------------------------------------------------
	bool Window::IsMaximized() const
	{
		return ::IsZoomed(m_HWnd) != FALSE;
	}

	//---------------------------------------------------------------------
	bool Window::IsOnResizeBorder() const
	{
		if (!m_bFrameless || !m_HWnd || ::IsZoomed(m_HWnd))
			return false;

		POINT cursor;
		if (!::GetCursorPos(&cursor))
			return false;

		RECT rc;
		if (!::GetWindowRect(m_HWnd, &rc))
			return false;

		const int borderX = ::GetSystemMetrics(SM_CXFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER);
		const int borderY = ::GetSystemMetrics(SM_CYFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER);
		const int systemBorder = max(borderX, borderY);
		const int bw = max(systemBorder + 4, 12);

		const bool left   = cursor.x < rc.left + bw;
		const bool right  = cursor.x >= rc.right - bw;
		const bool top    = cursor.y < rc.top + bw;
		const bool bottom = cursor.y >= rc.bottom - bw;

		return left || right || top || bottom;
	}

	//---------------------------------------------------------------------
	void Window::ApplyRoundedCorners()
	{
		if (!m_bFrameless || !m_HWnd)
			return;
		// Win11 native rounding - DWMWA_WINDOW_CORNER_PREFERENCE = 33, DWMWCP_ROUND = 2
		// Dynamic load so Win10 / old SDKs still run. No SetWindowRgn / DwmExtendFrame -
		// those clip the 12px hit-test borders and break resizing.
		constexpr DWORD kDwmwaWindowCornerPreference = 33;
		constexpr DWORD kDwmwcpRound = 2;
		HMODULE hDwm = ::LoadLibraryW(L"dwmapi.dll");
		if (!hDwm) return;
		using PFN_DwmSetWindowAttribute = HRESULT(WINAPI*)(HWND, DWORD, LPCVOID, DWORD);
		auto pfn = reinterpret_cast<PFN_DwmSetWindowAttribute>(::GetProcAddress(hDwm, "DwmSetWindowAttribute"));
		if (pfn)
		{
			DWORD pref = kDwmwcpRound;
			pfn(m_HWnd, kDwmwaWindowCornerPreference, &pref, sizeof(pref));
		}
		::FreeLibrary(hDwm);
	}

	//---------------------------------------------------------------------
	bool Window::HasPendingResize() const
	{
		return m_iResizeWidth != 0 && m_iResizeHeight != 0;
	}

	//---------------------------------------------------------------------
	void Window::ConsumeResize(uint32_t& a_iOutWidth, uint32_t& a_iOutHeight)
	{
		a_iOutWidth = m_iResizeWidth;
		a_iOutHeight = m_iResizeHeight;
		m_iResizeWidth = 0;
		m_iResizeHeight = 0;
	}

	//---------------------------------------------------------------------
	std::string Window::ConsumeDroppedFile()
	{
		std::string path = std::move(m_sDroppedFile);
		m_sDroppedFile.clear();
		return path;
	}

	//---------------------------------------------------------------------
	LRESULT CALLBACK Window::WndProc(HWND a_HWnd, UINT a_iMsg, WPARAM a_WParam, LPARAM a_LParam)
	{
		Window* window = nullptr;

		if (a_iMsg == WM_NCCREATE)
		{
			window = reinterpret_cast<Window*>(reinterpret_cast<CREATESTRUCTW*>(a_LParam)->lpCreateParams);
			::SetWindowLongPtrW(a_HWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		}
		else
		{
			window = reinterpret_cast<Window*>(::GetWindowLongPtrW(a_HWnd, GWLP_USERDATA));
		}

		if (window)
		{
			window->m_HWnd = a_HWnd;
			return window->HandleMessage(a_iMsg, a_WParam, a_LParam);
		}

		return ::DefWindowProcW(a_HWnd, a_iMsg, a_WParam, a_LParam);
	}

	//---------------------------------------------------------------------
	LRESULT Window::HandleMessage(UINT a_iMsg, WPARAM a_WParam, LPARAM a_LParam)
	{
		// --- Frameless handling MUST come before the ImGui hook, otherwise ImGui can swallow WM_NCHITTEST/WM_NCCALCSIZE ---
		if (m_bFrameless)
		{
			if (a_iMsg == WM_NCCALCSIZE && a_WParam == TRUE)
			{
				// Remove standard frame (title bar/border) but keep the sizing border.
				// When maximized we must not return 0 without insetting, otherwise the
				// window covers the taskbar. Insetting is handled via WM_GETMINMAXINFO,
				// but we also adjust here for multi-monitor correctness.
				return 0;
			}

			if (a_iMsg == WM_NCHITTEST)
			{
				POINTS cursor = MAKEPOINTS(a_LParam);
				RECT rc;
				GetWindowRect(m_HWnd, &rc);

				// Use system frame metrics so hit area matches the invisible sizing border
				// Make it intentionally thicker than the default 8px so it's easier to grab on a borderless window
				const int borderX = ::GetSystemMetrics(SM_CXFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER);
				const int borderY = ::GetSystemMetrics(SM_CYFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER);
				const int systemBorder = max(borderX, borderY); // typically 8px
				const int bw = max(systemBorder + 4, 12); // 12px minimum, ~12-16px effective for easy resizing

				const bool left   = cursor.x < rc.left + bw;
				const bool right  = cursor.x >= rc.right - bw;
				const bool top    = cursor.y < rc.top + bw;
				const bool bottom = cursor.y >= rc.bottom - bw;

				// Resize borders - disabled when maximized
				if (!::IsZoomed(m_HWnd))
				{
					if (top)
					{
						if (left)  return HTTOPLEFT;
						if (right) return HTTOPRIGHT;
						return HTTOP;
					}
					if (bottom)
					{
						if (left)  return HTBOTTOMLEFT;
						if (right) return HTBOTTOMRIGHT;
						return HTBOTTOM;
					}
					if (left)  return HTLEFT;
					if (right) return HTRIGHT;
				}
				else
				{
					// When maximized, no resize - but still allow drag via caption below
				}

				// --- Dragging: treat top bar as title bar ---
				// 50px toolbar height, exclude right 400px where window buttons live
				// Return HTCAPTION so OS handles move, snap, double-click maximize, and Aero.
				const int toolbarHeight = 50;
				const int buttonAreaWidth = 400;
				if (cursor.y < rc.top + toolbarHeight && cursor.x < rc.right - buttonAreaWidth)
				{
					return HTCAPTION;
				}

				return HTCLIENT;
			}

			if (a_iMsg == WM_GETMINMAXINFO)
			{
				// Enforce a minimum track size and fix auto-maximize covering the taskbar
				MINMAXINFO* mmi = reinterpret_cast<MINMAXINFO*>(a_LParam);
				mmi->ptMinTrackSize.x = 640;
				mmi->ptMinTrackSize.y = 400;

				HMONITOR hMon = ::MonitorFromWindow(m_HWnd, MONITOR_DEFAULTTONEAREST);
				MONITORINFO mi = { sizeof(mi) };
				if (::GetMonitorInfoW(hMon, &mi))
				{
					mmi->ptMaxPosition.x = mi.rcWork.left - mi.rcMonitor.left;
					mmi->ptMaxPosition.y = mi.rcWork.top - mi.rcMonitor.top;
					mmi->ptMaxSize.x = mi.rcWork.right - mi.rcWork.left;
					mmi->ptMaxSize.y = mi.rcWork.bottom - mi.rcWork.top;
				}
				return 0;
			}
		}

		// Programmatic drag via Window::StartDrag() - kept for ImGui explicit calls
		if (m_bDragging)
		{
			switch (a_iMsg)
			{
				case WM_MOUSEMOVE:
				{
					POINT cursor;
					GetCursorPos(&cursor);
					SetWindowPos(m_HWnd, nullptr, cursor.x - m_iDragOffset.x, cursor.y - m_iDragOffset.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
					return 0;
				}
				case WM_LBUTTONUP:
				{
					m_bDragging = false;
					ReleaseCapture();
					return 0;
				}
			}
		}

		if (a_iMsg == WM_GETMINMAXINFO)
		{
			// Enforce a minimum track size and fix auto-maximize covering the taskbar
			MINMAXINFO* mmi = reinterpret_cast<MINMAXINFO*>(a_LParam);
			mmi->ptMinTrackSize.x = 640;
			mmi->ptMinTrackSize.y = 400;

			// When frameless, the default maximized size would cover the taskbar.
			// Clamp to the monitor work area.
			HMONITOR hMon = ::MonitorFromWindow(m_HWnd, MONITOR_DEFAULTTONEAREST);
			MONITORINFO mi = { sizeof(mi) };
			if (::GetMonitorInfoW(hMon, &mi))
			{
				mmi->ptMaxPosition.x = mi.rcWork.left - mi.rcMonitor.left;
				mmi->ptMaxPosition.y = mi.rcWork.top - mi.rcMonitor.top;
				mmi->ptMaxSize.x = mi.rcWork.right - mi.rcWork.left;
				mmi->ptMaxSize.y = mi.rcWork.bottom - mi.rcWork.top;
			}
			return 0;
		}

		// Let Win32 dictate cursor on resize borders - before ImGui hook or ImGui stomps sizing cursors
		if (a_iMsg == WM_SETCURSOR && m_bFrameless && !::IsZoomed(m_HWnd))
		{
			WORD hit = LOWORD(a_LParam);
			if (hit == HTLEFT || hit == HTRIGHT || hit == HTTOP || hit == HTBOTTOM ||
				hit == HTTOPLEFT || hit == HTTOPRIGHT || hit == HTBOTTOMLEFT || hit == HTBOTTOMRIGHT)
			{
				return ::DefWindowProcW(m_HWnd, a_iMsg, a_WParam, a_LParam);
			}
			if (IsOnResizeBorder())
				return ::DefWindowProcW(m_HWnd, a_iMsg, a_WParam, a_LParam);
		}

		if (m_MessageHook && m_MessageHook(m_HWnd, a_iMsg, a_WParam, a_LParam))
		{
			return 0;
		}

		switch (a_iMsg)
		{
			case WM_SIZE:
			{
				if (a_WParam == SIZE_MINIMIZED)
				{
					return 0;
				}
				m_iResizeWidth = static_cast<uint32_t>(LOWORD(a_LParam));
				m_iResizeHeight = static_cast<uint32_t>(HIWORD(a_LParam));
				return 0;
			}
			case WM_DWMCOMPOSITIONCHANGED:
			{
				if (m_bFrameless)
					ApplyRoundedCorners();
				break;
			}
			case WM_SYSCOMMAND:
			{
				if ((a_WParam & 0xfff0) == SC_KEYMENU)
				{
					return 0;
				}
				break;
			}
			case WM_DESTROY:
			{
				::PostQuitMessage(0);
				return 0;
			}
			case WM_DROPFILES:
			{
				HDROP hDrop = reinterpret_cast<HDROP>(a_WParam);
				wchar_t path[MAX_PATH];
				if (::DragQueryFileW(hDrop, 0, path, MAX_PATH))
				{
					char utf8[MAX_PATH * 3];
					int len = ::WideCharToMultiByte(CP_UTF8, 0, path, -1, utf8, sizeof(utf8), nullptr, nullptr);
					if (len > 0) m_sDroppedFile = utf8;
				}
				::DragQueryPoint(hDrop, &m_ptDropPoint);
				::DragFinish(hDrop);
				return 0;
			}
		}
		return ::DefWindowProcW(m_HWnd, a_iMsg, a_WParam, a_LParam);
	}
}

