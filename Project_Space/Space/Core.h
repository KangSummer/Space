#pragma once
#include "define.h"

class CCore
{
	SINGLE(CCore);

private:

	HWND m_hWnd;

	ID3D11Device*								m_pDevice;
	ID3D11DeviceContext*				m_pDeviceContext;
	IDXGISwapChain*							m_pSwapChain;
	ID3D11Texture2D*						m_pDepthStencilBuffer;
	ID3D11RenderTargetView*		m_pRenderTargetView;
	ID3D11DepthStencilView*			m_pDepthStencilView;
	ID3D11RasterizerState*				m_pResterizerState;

	D3D11_VIEWPORT						m_ScreenViewport;

	Ressolution										m_Resolution;

	UINT													m_iQualityLevel;

public:
	Ressolution GetClientSize() const { return m_Resolution; }

	void ClearView(float(&_fColor)[4])
	{
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, _fColor);
		//m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}

	void Present()
	{
		m_pSwapChain->Present(0, 0);
	}

public:
	int init(HWND _hWnd);
	void update();
};
