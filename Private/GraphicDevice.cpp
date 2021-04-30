#include "stdafx.h"
#include "GraphicDevice.h"

IMPLEMENT_SINGLETON(CGraphicDevice)

CGraphicDevice::CGraphicDevice()
{
}


CGraphicDevice::~CGraphicDevice()
{
	Release_Graphic_Device();
}

HRESULT CGraphicDevice::Ready_Graphic_Device(WINMODE eMode)
{
	D3DCAPS9 DeviceCap;
	ZeroMemory(&DeviceCap, sizeof(D3DCAPS9));
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCap)))
	{
		ERR_MSG(L"Create SDK Failed");
		return E_FAIL;
	}
	DWORD vp = 0;
	if (DeviceCap.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferWidth = 800;
	d3dpp.BackBufferHeight = 600;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;// 여기서 1은 총 2개의 백버퍼를 사용하겠다. 즉 디폴트로 한개 생김. 거기에 +1 임 

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // 스왑 체인 방식을 사용하겠다. -> 더블버퍼링의 상위 호환버전. 
	d3dpp.hDeviceWindow = g_hWND;
	d3dpp.Windowed = TRUE;// TRUE일 경우 창모드, FALSE일 경우 전체화면 모드 MFC에선 창모드로 사용. 
	d3dpp.EnableAutoDepthStencil = eMode;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWND, vp, &d3dpp, &m_pDevice)))
	{
		ERR_MSG(L"Device Creating Failed");
		return E_FAIL;
	}
	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		ERR_MSG(L"Sprite Creating Failed");
		return E_FAIL;
	}
	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));
	tFontInfo.Height = 16;
	tFontInfo.Width = 8;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"맑은 고딕");

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		ERR_MSG(L"Creating Failed Font");
		return E_FAIL;
	}
	if (FAILED(D3DXCreateLine(m_pDevice, &m_pLine)))
	{
		ERR_MSG(L"Creating Failed Line");
		return E_FAIL;
	}

	return S_OK;
}

void CGraphicDevice::Release_Graphic_Device()
{
	if (m_pFont)
		m_pFont->Release();
	if (m_pSprite)
		m_pSprite->Release();
	if (m_pDevice)
		m_pDevice->Release();
	if (m_pSDK)
		m_pSDK->Release();
	//Device 안에서 내부적으로 LPDIRECT3D9 참조해서 사용하고 있기 때문에 Device를 먼저 제거 후LPDIRECT3D9를 제거해줘야 누수가 남지 않는다.  
}

void CGraphicDevice::Render_Begin()
{
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 50, 50, 50), 1.f, 0);
	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

}

void CGraphicDevice::Render_End(HWND hWND /*= nullptr*/)
{
	m_pSprite->End();
	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, hWND, nullptr);
}
