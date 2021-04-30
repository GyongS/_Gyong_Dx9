#include "stdafx.h"
#include "Time_Manager.h"


IMPLEMENT_SINGLETON(CTime_Manager)
CTime_Manager::CTime_Manager()
	:m_fDeltaTime(0.f)
{
	ZeroMemory(&m_CPUTick, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_BeginTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_EndTime, sizeof(LARGE_INTEGER));

}


CTime_Manager::~CTime_Manager()
{
}

HRESULT CTime_Manager::Ready_TimeManager()
{
	QueryPerformanceFrequency(&m_CPUTick); // �ʴ� CPU�� �������� ��ȯ���ִ� �Լ�. 
	QueryPerformanceCounter(&m_BeginTime); //���� ��� �����ߴ���
	QueryPerformanceCounter(&m_EndTime);
	return S_OK;
}

void CTime_Manager::Update_TimeManager()
{
	QueryPerformanceFrequency(&m_CPUTick);
	QueryPerformanceCounter(&m_EndTime);
	m_fDeltaTime = float(m_EndTime.QuadPart - m_BeginTime.QuadPart) / m_CPUTick.QuadPart;
	m_BeginTime.QuadPart = m_EndTime.QuadPart;
}
