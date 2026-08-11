#pragma once

#include "WINPCH.h"

#include <cstdint>
#include <string>

namespace humongousexplorer::win32
{
	//---------------------------------------------------------------------
	// Window
	//---------------------------------------------------------------------
	/// <summary>
	/// Represents a Win32 application window, handling class registration,
	/// creation, message pumping and resize tracking.
	/// </summary>
	class Window
	{
	public:
		/// <summary>
		/// Signature for an optional message hook, called before default handling.
		/// </summary>
		/// <returns>Non-zero if the message was handled and default processing should be skipped.</returns>
		using MessageHook = LRESULT(WINAPI*)(HWND, UINT, WPARAM, LPARAM);

		Window() = default;
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		/// <summary>
		/// Registers the window class and creates the window.
		/// </summary>
		/// <param name="a_HInstance">The application instance handle.</param>
		/// <param name="a_iWidth">The desired window width in pixels.</param>
		/// <param name="a_iHeight">The desired window height in pixels.</param>
		/// <param name="a_sTitle">The window title.</param>
		/// <returns>True if the window was created successfully, otherwise false.</returns>
		bool Initialize(HINSTANCE a_HInstance, int a_iWidth, int a_iHeight, const wchar_t* a_sTitle);

		/// <summary>
		/// Destroys the window and unregisters the class.
		/// </summary>
		void Destroy();

		/// <summary>
		/// Processes pending messages, dispatching them to the window procedure.
		/// </summary>
		/// <returns>False once a quit message has been received, otherwise true.</returns>
		bool ProcessMessages();

		/// <summary>
		/// Makes the window visible and updates it.
		/// </summary>
		/// <param name="a_ShowCommand">The SW_* command controlling how the window is displayed.</param>
		void Show(int a_iShowCmd);

		/// <summary>
		/// Installs an optional hook invoked for every window message before default handling.
		/// </summary>
		/// <param name="a_Hook">The hook to invoke, or nullptr to clear it.</param>
		void SetMessageHook(MessageHook a_Hook);

		/// <summary>
		/// Retrieves the native window handle.
		/// </summary>
		HWND GetHandle() const;

		/// <summary>
		/// Checks whether a resize was queued and not yet consumed.
		/// </summary>
		bool HasPendingResize() const;

		/// <summary>
		/// Retrieves and clears the queued client area size.
		/// </summary>
		void ConsumeResize(uint32_t& a_iOutWidth, uint32_t& a_iOutHeight);

		bool HasDroppedFile() const { return !m_sDroppedFile.empty(); }
		std::string ConsumeDroppedFile();
		POINT GetDroppedFilePosition() const { return m_ptDropPoint; }

	private:
		/// <summary>
		/// Static window procedure forwarding to the instance.
		/// </summary>
		static LRESULT CALLBACK WndProc(HWND a_HWnd, UINT a_iMsg, WPARAM a_WParam, LPARAM a_LParam);

		/// <summary>
		/// Instance window procedure handling window messages.
		/// </summary>
		LRESULT HandleMessage(UINT a_iMsg, WPARAM a_WParam, LPARAM a_LParam);

		HWND m_HWnd = nullptr; /// The native window handle.
		HINSTANCE m_HInstance = nullptr; /// The application instance handle.
		std::wstring m_wsClassName; /// The registered window class name.
		MessageHook m_MessageHook = nullptr; /// Optional external message hook.
		uint32_t m_iResizeWidth = 0; /// Queued client area width, 0 if no resize.
		uint32_t m_iResizeHeight = 0; /// Queued client area height, 0 if no resize.
		std::string m_sDroppedFile; /// Last file dropped onto the window.
		POINT m_ptDropPoint = {}; /// Client-area position of the last drop.
	};
}
