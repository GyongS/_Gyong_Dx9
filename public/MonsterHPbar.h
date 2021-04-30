#pragma once
#include "GameObject.h"
class CMonsterHPbar : public CGameObject
{
public:
	CMonsterHPbar();
	virtual ~CMonsterHPbar();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	float Get_BackFillAmount() { return m_fBackHPFill; }

private:
	void MonsterBackHp(float _Infill);

private:
	float m_fFill;
	float m_fBackFill;

	float m_fFillAmount;
	float m_fBackHPFill;

	float m_fRatioX;
};

