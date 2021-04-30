#pragma once
#include "GameObject.h"
class CEvent_Text : public CGameObject
{
public:
	CEvent_Text();
	virtual ~CEvent_Text();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	void Tutorial_0_Event();
	void Tutorial_1_Event();
	void Tutorial_2_Event();
	void BossDead_Event();
private:
	wstring m_strBuff;
	wstring m_strSaveText[64];
	wstring m_strCurText;
private:
	float m_fTimer;
	int   m_iTextCnt;
	int   m_iSaveIndex;
	int   m_iIndex;
};

