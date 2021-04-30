#pragma once
#include "GameObject.h"
class CSkill_Icon_S : public CGameObject
{
public:
	CSkill_Icon_S();
	virtual ~CSkill_Icon_S();

	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Late_Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	void Change_Skill_Icon_S();
};

