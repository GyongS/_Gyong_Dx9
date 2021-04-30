#pragma once
#include "GameObject.h"
class CBossHPBar : public CGameObject
{
public:
	CBossHPBar();
	virtual ~CBossHPBar();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	float Get_BackFillAmount() { return m_fBackHPFill; }

private:
	void BossBackHp(float _Infill);

private:
	float m_fFill;
	float m_fBackFill;

	float m_fFillAmount;
	float m_fBackHPFill;
	float m_fMinusX;

	TCHAR m_szbuff[64];
};

