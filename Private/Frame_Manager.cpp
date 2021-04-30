#include "stdafx.h"
#include "Frame_Manager.h"
#include "GraphicDevice.h"
#include "Time_Manager.h"

CFrame_Manager::CFrame_Manager()
	: m_fDeltaTime(0.f)
	, m_uiFPS(0)
	, m_fFPSTime(0.f)
	, m_szFPS(L"")
{
	ZeroMemory(&m_CPUTick, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_BeginTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_EndTime, sizeof(LARGE_INTEGER));
}


CFrame_Manager::~CFrame_Manager()
{
}

HRESULT CFrame_Manager::Ready_TimeManager(float fFPS)
{
	m_fSecondPerFrame = 1.f / fFPS;
	QueryPerformanceFrequency(&m_CPUTick); // 초당 CPU의 진동수를 반환해주는 함수. 
	QueryPerformanceCounter(&m_BeginTime); //현재 몇번 진동했는지
	QueryPerformanceCounter(&m_EndTime);

	return S_OK;
}

void CFrame_Manager::RenderFPS_TimeManager()
{
	m_fFPSTime += CTime_Manager::Get_Instance()->Get_DeltaTime();
	if (1.f < m_fFPSTime)
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_uiFPS);
		m_uiFPS = 0;
		m_fFPSTime -= 1;
	}
	// SetWindowText(g_hWND, m_szFPS); 

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 10.f, 10.f, 0.f);
	CGraphicDevice::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	CGraphicDevice::Get_Instance()->Get_Font()->DrawTextW(CGraphicDevice::Get_Instance()->Get_Sprite(), m_szFPS, lstrlen(m_szFPS), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
}

bool CFrame_Manager::Frame_Lock()
{
	QueryPerformanceCounter(&m_EndTime);

	m_fDeltaTime += float(m_EndTime.QuadPart - m_BeginTime.QuadPart) / m_CPUTick.QuadPart;
	m_BeginTime.QuadPart = m_EndTime.QuadPart;
	
	if (m_fSecondPerFrame < m_fDeltaTime)
	{
		QueryPerformanceFrequency(&m_CPUTick);
		m_fDeltaTime -= m_fSecondPerFrame;
		++m_uiFPS;
		return true;
	}
	return false;
}
