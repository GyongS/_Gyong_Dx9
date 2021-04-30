#pragma once
#include "Scene.h"
class CLoadingScene :
	public CScene
{
public:
	CLoadingScene();
	virtual ~CLoadingScene();

	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual int Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

public:
	static unsigned __stdcall LoadImageFunc(LPVOID pArg);
private:
	// 쓰레드 관리할 핸들, 크리티컬 섹션도 써볼께 뭐 예제라서 
	HANDLE m_hThread;
	CRITICAL_SECTION m_CriticalSection;

	float m_fLogoAlpha;
	bool m_bLoading;
};

