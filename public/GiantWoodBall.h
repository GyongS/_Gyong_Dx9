#pragma once
#include "GameObject.h"
class CGiantWoodBall : public CGameObject
{
public:
	CGiantWoodBall();
	virtual ~CGiantWoodBall();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	float m_fLifeTime;
};

