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

	// FPS 체크용 변수 , FPS 시간값 가지고 있을 변수, FPS 띄어줄 문자열 보관할 변수. 
	unsigned int	m_uiFPS;
	float			m_fFPSTime;
	TCHAR			m_szFPS[64];
};

