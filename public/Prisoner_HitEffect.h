#pragma once
#include "GameObject.h"
class CPrisoner_HitEffect : public CGameObject
{
public:
	CPrisoner_HitEffect();
	virtual ~CPrisoner_HitEffect();

	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Late_Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};

