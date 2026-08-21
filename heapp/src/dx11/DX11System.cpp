#include "DX11System.h"

#include "logger/Logger.h"

namespace humongousexplorer::dx11
{
	//---------------------------------------------------------------------
	// DX11System
	//---------------------------------------------------------------------
	dx11::DX11System& GetDX11System()
	{
		static humongousexplorer::dx11::DX11System dx11;
		return dx11;
	}

	//---------------------------------------------------------------------
	bool DX11System::Initialize(HWND a_HWnd)
	{
		if (m_pDevice)
		{
			// Already initialized.
			return false;
		}

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Width = 0;
		swapChainDesc.BufferDesc.Height = 0;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = a_HWnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		UINT createDeviceFlags = 0;
		const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
		D3D_FEATURE_LEVEL featureLevel;

		HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pDevice, &featureLevel, &m_pDeviceContext);
		if (res == DXGI_ERROR_UNSUPPORTED)
		{
			res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pDevice, &featureLevel, &m_pDeviceContext);
		}
		if (res != S_OK)
		{
			return false;
		}

		CreateRenderTarget();

		LOG(LOGSEVERITY_SUCCESS, "Initialized dx11 system.");

		return System::Initialize();
	}

	//---------------------------------------------------------------------
	bool DX11System::Destroy()
	{
		CleanupRenderTarget();
		if (m_pSwapChain) { m_pSwapChain->Release(); m_pSwapChain = nullptr; }
		if (m_pDeviceContext) { m_pDeviceContext->Release(); m_pDeviceContext = nullptr; }
		if (m_pDevice) { m_pDevice->Release(); m_pDevice = nullptr; }

		return System::Destroy();
	}

	//---------------------------------------------------------------------
	void DX11System::BeginFrame(const float a_aClearColor[4])
	{
		m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, a_aClearColor);
	}

	//---------------------------------------------------------------------
	void DX11System::EndFrame(UINT a_iSyncInterval)
	{
		HRESULT hr = m_pSwapChain->Present(a_iSyncInterval, 0);
		m_bSwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
	}

	//---------------------------------------------------------------------
	void DX11System::Resize(uint32_t a_iWidth, uint32_t a_iHeight)
	{
		CleanupRenderTarget();
		m_pSwapChain->ResizeBuffers(0, a_iWidth, a_iHeight, DXGI_FORMAT_UNKNOWN, 0);
		CreateRenderTarget();
	}

	//---------------------------------------------------------------------
	ID3D11Device* DX11System::GetDevice() const
	{
		return m_pDevice;
	}

	//---------------------------------------------------------------------
	ID3D11DeviceContext* DX11System::GetDeviceContext() const
	{
		return m_pDeviceContext;
	}

	//---------------------------------------------------------------------
	ID3D11ShaderResourceView* DX11System::CreateTexture(const void* a_pPixels, int a_iWidth, int a_iHeight)
	{
		if (!m_pDevice || !a_pPixels || a_iWidth <= 0 || a_iHeight <= 0)
		{
			return nullptr;
		}

		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = a_iWidth;
		texDesc.Height = a_iHeight;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = a_pPixels;
		initData.SysMemPitch = a_iWidth * 4;

		ID3D11Texture2D* texture = nullptr;
		HRESULT hr = m_pDevice->CreateTexture2D(&texDesc, &initData, &texture);
		if (FAILED(hr) || !texture)
		{
			return nullptr;
		}

		ID3D11ShaderResourceView* srv = nullptr;
		hr = m_pDevice->CreateShaderResourceView(texture, nullptr, &srv);
		texture->Release();

		if (FAILED(hr) || !srv)
		{
			return nullptr;
		}

		return srv;
	}

	//---------------------------------------------------------------------
	void DX11System::CreateRenderTarget()
	{
		ID3D11Texture2D* backBuffer = nullptr;
		m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
		if (backBuffer)
		{
			m_pDevice->CreateRenderTargetView(backBuffer, nullptr, &m_pRenderTargetView);
			backBuffer->Release();
		}
	}

	//---------------------------------------------------------------------
	void DX11System::CleanupRenderTarget()
	{
		if (m_pRenderTargetView)
		{
			m_pRenderTargetView->Release();
			m_pRenderTargetView = nullptr;
		}
	}
}