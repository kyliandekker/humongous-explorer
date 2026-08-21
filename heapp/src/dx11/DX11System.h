#pragma once

#include "core/System.h"
#include "win32/WINPCH.h"

#include <d3d11.h>

namespace humongousexplorer::dx11
{
	class DX11System;
	extern DX11System& GetDX11System();

	//---------------------------------------------------------------------
	// DX11System
	//---------------------------------------------------------------------
	/// <summary>
	/// Manages the Direct3D 11 device, swap chain and render target for a window.
	/// </summary>
	class DX11System : public core::System
	{
	public:
		/// <summary>
		/// Creates the D3D11 device, swap chain and render target for the given window.
		/// </summary>
		/// <param name="a_HWnd">The window the swap chain should present to.</param>
		/// <returns>True if the device was created successfully, otherwise false.</returns>
		bool Initialize(HWND a_HWnd);

		/// <summary>
		/// Releases the render target, swap chain and device.
		/// </summary>
		bool Destroy() override;

		/// <summary>
		/// Begins a new frame: clears the render target and binds it.
		/// </summary>
		/// <param name="a_aClearColor">The RGBA color to clear the back buffer with.</param>
		void BeginFrame(const float a_aClearColor[4]);

		/// <summary>
		/// Presents the back buffer to the swap chain, handling occlusion and resizes.
		/// </summary>
		/// <param name="a_iSyncInterval">The number of vsync frames to wait for.</param>
		void EndFrame(UINT a_iSyncInterval);

		/// <summary>
		/// Resizes the back buffers and recreates the render target.
		/// </summary>
		/// <param name="a_iWidth">The new client area width.</param>
		/// <param name="a_iHeight">The new client area height.</param>
		void Resize(uint32_t a_iWidth, uint32_t a_iHeight);

		/// <summary>
		/// Retrieves the D3D11 device.
		/// </summary>
		ID3D11Device* GetDevice() const;

		/// <summary>
		/// Retrieves the D3D11 device context.
		/// </summary>
		ID3D11DeviceContext* GetDeviceContext() const;

		/// <summary>
		/// Creates a shader resource view from RGBA pixel data.
		/// </summary>
		/// <param name="a_pPixels">Pointer to RGBA pixel data (4 bytes per pixel).</param>
		/// <param name="a_iWidth">Width of the image in pixels.</param>
		/// <param name="a_iHeight">Height of the image in pixels.</param>
		/// <returns>The created shader resource view, or nullptr on failure.</returns>
		ID3D11ShaderResourceView* CreateTexture(const void* a_pPixels, int a_iWidth, int a_iHeight);
	private:
		/// <summary>
		/// Creates the render target view for the current back buffer.
		/// </summary>
		void CreateRenderTarget();

		/// <summary>
		/// Releases the render target view.
		/// </summary>
		void CleanupRenderTarget();

		ID3D11Device* m_pDevice = nullptr; /// The D3D11 device.
		ID3D11DeviceContext* m_pDeviceContext = nullptr; /// The D3D11 device context.
		IDXGISwapChain* m_pSwapChain = nullptr; /// The swap chain.
		ID3D11RenderTargetView* m_pRenderTargetView = nullptr; /// The main render target view.
		bool m_bSwapChainOccluded = false; /// Whether the swap chain is currently occluded.
	};
}