#pragma once
#include "GameObject.h"
class CDamageFont :	public CGameObject
{
public:
	CDamageFont();
	virtual ~CDamageFont();

	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Late_Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void Set_DamageFont(int _InDamage) { m_iDamage = _InDamage; }

public:
	void Render_Damage(int InDamage);

public:
	static CDamageFont* Create();

private:
	float m_fJumpForce;
	float m_fJumpTime;
	float m_fJumpY;
	float m_fGravity;
	float m_fLifeTime;

private:
	int m_iDamage;

};

