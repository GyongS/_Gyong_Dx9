#pragma once
#include "GameObject.h"
class CStone_HitEffect : public CGameObject
{
public:
	CStone_HitEffect();
	virtual ~CStone_HitEffect();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};

