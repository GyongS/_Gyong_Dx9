#pragma once
#include "GameObject.h"
class CIceCrossShotEffect : public CGameObject
{
public:
	CIceCrossShotEffect();
	virtual ~CIceCrossShotEffect();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};

