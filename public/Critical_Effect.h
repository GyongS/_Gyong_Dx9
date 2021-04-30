#pragma once
#include "GameObject.h"
class CCritical_Effect : public CGameObject
{
public:
	CCritical_Effect();
	virtual ~CCritical_Effect();

	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Late_Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

};

