#pragma once
#include "GameObject.h"
class CIceParticle : public CGameObject
{
public:
	CIceParticle();
	virtual ~CIceParticle();

	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Late_Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	float m_fGravity;
	float m_fTime;
	float m_fLifeTime;
};

