#pragma once
#include "GameObject.h"
class CSwapCoolDownEffect : public CGameObject
{
public:
	CSwapCoolDownEffect();
	virtual ~CSwapCoolDownEffect();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};

