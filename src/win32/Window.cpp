#include "Window.h"

#include "logger/Logger.h"

namespace humongousexplorer::win32
{
	//---------------------------------------------------------------------
	// Window
	//---------------------------------------------------------------------
	Window::~Window()
	{
		Destroy();
	}

	//---------------------------------------------------------------------
	bool Window::Initialize(HINSTANCE a_HInstance, int a_iWidth, int a_iHeight, const wchar_t* a_sTitle)
	{
		m_HInstance = a_HInstance;

		wchar_t className[64];
		swprintf_s(className, L"HumongousExplorer_%08X", GetCurrentProcessId());
		m_wsClassName = className;

		WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, &Window::WndProc, 0L, 0L, a_HInstance, nullptr, nullptr, nullptr, nullptr, m_wsClassName.c_str(), nullptr };

		if (!::RegisterClassExW(&wc))
		{
			DWORD err = ::GetLastError();
			WNDCLASSEXW existing;
			if (err != ERROR_CLASS_ALREADY_EXISTS || ::GetClassInfoExW(a_HInstance, m_wsClassName.c_str(), &existing) == 0)
			{
				LOGF(LOGSEVERITY_ERROR, "Failed registering window class %s, error 0x%08X", m_wsClassName.c_str(), err);
				return false;
			}
		}

		m_HWnd = ::CreateWindowW(m_wsClassName.c_str(), a_sTitle, WS_OVERLAPPEDWINDOW, 100, 100, a_iWidth, a_iHeight, nullptr, nullptr, a_HInstance, this);
		if (!m_HWnd)
		{
			LOGF(LOGSEVERITY_ERROR, "Failed creating window, error 0x%08X", ::GetLastError());
			::UnregisterClassW(m_wsClassName.c_str(), a_HInstance);
			return false;
		}

		LOG(LOGSEVERITY_SUCCESS, "Initialized window.");

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
			if (msg.message == WM_QUIT)
			{
				running = false;
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
		if (m_MessageHook && m_MessageHook(m_HWnd, a_iMsg, a_WParam, a_LParam))
		{
			return 0;
		}

		switch (a_iMsg)
		{
			case WM_SIZE:
				if (a_WParam == SIZE_MINIMIZED)
				{
					return 0;
				}
				m_iResizeWidth = static_cast<uint32_t>(LOWORD(a_LParam));
				m_iResizeHeight = static_cast<uint32_t>(HIWORD(a_LParam));
				return 0;
			case WM_SYSCOMMAND:
				if ((a_WParam & 0xfff0) == SC_KEYMENU)
				{
					return 0;
				}
				break;
			case WM_DESTROY:
				::PostQuitMessage(0);
				return 0;
		}
		return ::DefWindowProcW(m_HWnd, a_iMsg, a_WParam, a_LParam);
	}
}