#pragma once
#include "GameObject.h"
class CDoubleJumpEffect : public CGameObject
{
public:
	CDoubleJumpEffect();
	virtual ~CDoubleJumpEffect();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};

