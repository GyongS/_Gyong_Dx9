#pragma once
#include "GameObject.h"
class CFreeze_State : public CGameObject
{
public:
	CFreeze_State();
	virtual ~CFreeze_State();
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	float m_fLifeTime;
};

