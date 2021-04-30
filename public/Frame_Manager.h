#pragma once
class CFrame_Manager
{
public:
	CFrame_Manager();
	~CFrame_Manager();

public:
	HRESULT Ready_TimeManager(float fFPS);
	void RenderFPS_TimeManager();
	bool  Frame_Lock();
private:
	LARGE_INTEGER	m_CPUTick;
	LARGE_INTEGER	m_BeginTime;
	LARGE_INTEGER	m_EndTime;
	float			m_fDeltaTime;
	float			m_fSecondPerFrame;

	// FPS üũ�� ���� , FPS �ð��� ������ ���� ����, FPS ����� ���ڿ� ������ ����. 
	unsigned int	m_uiFPS;
	float			m_fFPSTime;
	TCHAR			m_szFPS[64];
};

