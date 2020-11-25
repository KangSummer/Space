#include "Core.h"
#include "Timer.h"
#include "Device.h"

CCore::CCore() :
	m_hWnd(nullptr)
{
}

CCore::~CCore()
{
}

int CCore::init(HWND _hWnd)
{
	m_hWnd = _hWnd;

	RECT rt;
	GetWindowRect(m_hWnd, &rt);
	m_Resolution.fWidth				= (float)(rt.right - rt.left);
	m_Resolution.fHeight			= (float)(rt.bottom - rt.top);

	if (FAILED(CDevice::GetInst()->init()))
	{
		return E_FAIL;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 초기화 
	int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL eLv = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL eOut = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDevice(nullptr
		, D3D_DRIVER_TYPE_HARDWARE
		, nullptr
		, iFlag
		, &eLv
		, 1
		, D3D11_SDK_VERSION
		, &m_pDevice
		, &eOut
		, &m_pDeviceContext)))
	{
		return E_FAIL;
	}

	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_iQualityLevel);

	DXGI_SWAP_CHAIN_DESC tSwapDesc = {};

	tSwapDesc.BufferCount = 1;
	tSwapDesc.BufferDesc.Width = m_Resolution.fWidth;
	tSwapDesc.BufferDesc.Height = m_Resolution.fHeight;
	tSwapDesc.BufferDesc.RefreshRate.Numerator = 60;
	tSwapDesc.BufferDesc.RefreshRate.Denominator = 1;
	tSwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tSwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tSwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	tSwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	tSwapDesc.SampleDesc.Count = 4;
	tSwapDesc.SampleDesc.Quality = 0;

	tSwapDesc.OutputWindow = m_hWnd;
	tSwapDesc.Windowed = true;

	tSwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	IDXGIDevice*				pIDXDevice	= nullptr;
	IDXGIAdapter*			pAdapter		= nullptr;
	IDXGIFactory*				pFactory		= nullptr;

	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pIDXDevice);
	pIDXDevice->GetAdapter(&pAdapter);
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

	HRESULT hr = pFactory->CreateSwapChain(m_pDevice, &tSwapDesc, &m_pSwapChain);

	SAFE_RELEASE(pIDXDevice);
	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pFactory);

	ID3D11Texture2D* pRenderTargetTexture = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pRenderTargetTexture);
	SAFE_RELEASE(pRenderTargetTexture);

	m_pDevice->CreateRenderTargetView(pRenderTargetTexture, nullptr, &m_pRenderTargetView);
	int iError1 = GetLastError();

	D3D11_TEXTURE2D_DESC DepthStencilDesc;

	DepthStencilDesc.Width = (UINT)m_Resolution.fWidth;
	DepthStencilDesc.Height = (UINT)m_Resolution.fHeight;
	DepthStencilDesc.MipLevels = 1;
	DepthStencilDesc.ArraySize = 1;
	DepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilDesc.SampleDesc.Count = 4;
	DepthStencilDesc.SampleDesc.Quality = 0;
	DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;							// 메모리 사용 용도(읽기, 쓰기 관련)
	DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;			// Texture  가 DepthStencil 용도로 사용될 것을 알림

	m_pDevice->CreateTexture2D(&DepthStencilDesc, nullptr, &m_pDepthStencilBuffer);
	int iError = GetLastError();

	DepthStencilDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
	DepthStencilDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, nullptr, &m_pDepthStencilView);

	D3D11_RASTERIZER_DESC	RasterizerDesc;
	RasterizerDesc.FillMode = D3D11_FILL_SOLID;
	RasterizerDesc.CullMode = D3D11_CULL_NONE;
	RasterizerDesc.FrontCounterClockwise = FALSE;
	RasterizerDesc.DepthBias = 0;
	RasterizerDesc.DepthBiasClamp = 0;
	RasterizerDesc.SlopeScaledDepthBias = 0;
	RasterizerDesc.ScissorEnable = FALSE;
	RasterizerDesc.MultisampleEnable = FALSE;
	RasterizerDesc.AntialiasedLineEnable = FALSE;

	hr = m_pDevice->CreateRasterizerState(&RasterizerDesc, & m_pResterizerState);

	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	m_pDeviceContext->RSSetState(m_pResterizerState);

	D3D11_VIEWPORT tVP = {};
	tVP.Width = m_Resolution.fWidth;
	tVP.Height = m_Resolution.fHeight;

	tVP.TopLeftX = 0;
	tVP.TopLeftY = 0;

	tVP.MinDepth = 0.f;
	tVP.MinDepth = 1.f;

	m_pDeviceContext->RSSetViewports(1, &tVP);

	///////////////////////////////////////////////////////////////////////////////

	CTimer::GetInst()->init();

	return S_OK;
}

void CCore::update()
{
	CTimer::GetInst()->update();

	float arrColor[4] = { 1.f, 0.f, 0.f, .5f };
	ClearView(arrColor);
	
	Present();
}