#pragma once
#include "GameObject.h"
class CPlayerHPbar : public CGameObject
{
public:
	CPlayerHPbar();
	virtual ~CPlayerHPbar();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	float Get_BackFillAmount() { return m_fBackHPFill; }

private:
	void PlayerBackHp(float _Infill);

private:
	float m_fFill;
	float m_fBackFill;

	float m_fFillAmount;
	float m_fBackHPFill;

private:
	TCHAR			m_szbuff[64];	
};

