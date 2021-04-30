#pragma once
#include "GameObject.h"
class CSkillCoolDown : public CGameObject
{
	enum SKILL_ID
	{
		SKILL_A,
		SKILL_S,
		SKILL_D,
		END
	};
public:
	CSkillCoolDown();
	virtual ~CSkillCoolDown();

	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Late_Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:
	SKILL_ID m_eCurSkill_ID;

private:
	float m_fSkill_ACoolDown;
	float m_fSkill_SCoolDown;
	float m_fSkill_DCoolDown;
};

