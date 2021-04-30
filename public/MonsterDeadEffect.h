#pragma once
#include "GameObject.h"
class CMonsterDeadEffect : public CGameObject
{
public:
	CMonsterDeadEffect();
	virtual ~CMonsterDeadEffect();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
};

