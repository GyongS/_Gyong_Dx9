#pragma once
#include "Scene.h"
class CLoadingScene :
	public CScene
{
public:
	CLoadingScene();
	virtual ~CLoadingScene();

	// CScene��(��) ���� ��ӵ�
	virtual HRESULT Ready_Scene() override;
	virtual int Update_Scene() override;
	virtual void Late_Update_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

public:
	static unsigned __stdcall LoadImageFunc(LPVOID pArg);
private:
	// ������ ������ �ڵ�, ũ��Ƽ�� ���ǵ� �Ẽ�� �� ������ 
	HANDLE m_hThread;
	CRITICAL_SECTION m_CriticalSection;

	float m_fLogoAlpha;
	bool m_bLoading;
};

