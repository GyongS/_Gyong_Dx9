#pragma once
#include "GameObject.h"
class CStampingWave : public CGameObject
{
public:
	CStampingWave();
	virtual ~CStampingWave();

	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Late_Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	float m_fLifeTime;
};

