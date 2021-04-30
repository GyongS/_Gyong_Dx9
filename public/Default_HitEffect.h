#pragma once
#include "GameObject.h"
class CDefault_HitEffect : public CGameObject
{
public:
	CDefault_HitEffect();
	virtual ~CDefault_HitEffect();

	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Late_Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};

