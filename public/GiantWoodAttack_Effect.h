#pragma once
#include "GameObject.h"
class CGiantWoodAttack_Effect :	public CGameObject
{
public:
	CGiantWoodAttack_Effect();
	virtual ~CGiantWoodAttack_Effect();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};

