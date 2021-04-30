#pragma once
#include "GameObject.h"
class CGiantWood : public CGameObject
{
public:
	enum GiantWoodState
	{
		IDLE,
		ATTACK1,
		ATTACK2,
		DEAD,
		END
	};
public:
	CGiantWood();
	virtual ~CGiantWood();

	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Late_Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void Set_Damage(int _InDamage);
	void Set_bColorBlack(bool _bCheck) { m_bColorBlack = _bCheck; }
	void Set_HitIceAllow(bool _bCheck) { m_bHitIceAllow = _bCheck; }
	void Set_FreezeStage(bool _bCheck) { m_bFreezeState = _bCheck; }
	void Set_HitBroadSword(bool _bCheck) { m_bHitBroadSword = _bCheck; }
	void Set_HitGrenade(bool _bCheck) { m_bHitGrenade = _bCheck; }
public:
	bool& Get_HitIceAllow() { return m_bHitIceAllow; }
	bool& Get_FreezeState() { return m_bFreezeState; }
	bool& Get_HitGrenade() { return m_bHitGrenade; }
	bool& Get_HitBroadSword() { return m_bHitBroadSword; }

private:
	void Change_State();
	void Monster_Pattern();
	void STATE_IDLE();
	void STATE_ATTACK1();
	void STATE_ATTACK2();
	void HIT_ILSEOM_STATE();
	void FREEZE_STATE();
	void HIT_BROAD_SWORD();
	void HIT_GRENADE();

private:
	GiantWoodState m_eCurState;
	GiantWoodState m_ePreState;

	float m_fDelay;
	bool m_bAttack;
	bool  m_bColorBlack;
	bool  m_bHitIceAllow;
	bool  m_bFreezeState;
	bool  m_bHitBroadSword;
	bool  m_bHitGrenade;

	CGameObject* m_pMainHp;
	CGameObject* m_pServeHP;
	CGameObject* m_pHPBack;
	CGameObject* m_pMiniMapObj;
	CGameObject* m_pFreezeState;

};

