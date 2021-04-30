#pragma once
#include "GameObject.h"
class CGrenade_Effect :	public CGameObject
{
public:
	CGrenade_Effect();
	virtual ~CGrenade_Effect();

	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Late_Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};

