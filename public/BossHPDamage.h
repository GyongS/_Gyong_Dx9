#pragma once
#include "GameObject.h"
class CBossHPDamage : public CGameObject
{
public:
	CBossHPDamage();
	virtual ~CBossHPDamage();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	float& Get_FillAmount() { return m_fFillAmount; }

public:
	void Set_FillAmount(float _Infill) { m_fFillAmount = _Infill; }

private:
	float m_fFillAmount;
};

