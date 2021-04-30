#pragma once
#include "GameObject.h"
class CCage : public CGameObject
{
public:
	CCage();
	virtual ~CCage();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void Set_Damage(float _InDamage) { m_tStat.fCurHP -= _InDamage; }

public:
	float& Get_HP() { return m_tStat.fCurHP; }
	bool& Get_DestroyCheck() { return m_bDestroy;}
private:
	bool m_bDestroy;

};

