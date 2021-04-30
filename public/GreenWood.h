#pragma once
#include "GameObject.h"
class CGreenWood : public CGameObject
{
public:
	enum GreenWoodState
	{
		IDLE,
		WALK,
		ATTACK,
		HIT,
		HIT2,
		END
	};
public:
	CGreenWood();
	virtual ~CGreenWood();

	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Late_Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void Set_CheckGround() { m_bCheckGround = true; }
	void Set_Damage(int _InDamage);
	void Set_bColorBlack(bool _bCheck) { m_bColorBlack = _bCheck; }
	void Set_HitIceAllow(bool _bCheck) { m_bHitIceAllow = _bCheck; }
	void Set_FreezeStage(bool _bCheck) { m_bFreezeState = _bCheck; }
	void Set_HitBroadSword(bool _bCheck) { m_bHitBroadSword = _bCheck; }
	void Set_HitGrenade(bool _bCheck) { m_bHitGrenade = _bCheck; }

public:
	bool& Get_HitGrenade() { return m_bHitGrenade; }
	bool& Get_HitBroadSword() { return m_bHitBroadSword; }
	bool& Get_HitIceAllow() { return m_bHitIceAllow; }
	bool& Get_FreezeState() { return m_bFreezeState; }

private:
	void Change_State();
	void Check_LeftRight();
	void Monster_Pattern();
	void Set_State(GreenWoodState _eState) { m_eCurState = _eState; }
private:
	void IDLE_STATE();
	void WALK_STATE();
	void ATTACK_STATE();
	void HIT_ILSEOM_STATE();
	void FREEZE_STATE();
	void HIT_BROAD_SWORD();
	void HIT_GRENADE();

private:
	GreenWoodState m_eCurState;
	GreenWoodState m_ePreState;

private:
	int   m_iDir;
	float m_fDelay;
	bool  m_bAnimisPlaying;
	bool  m_bHitCheck;
	bool  m_bCreateCol;
	bool  m_bCheckGround;
	bool  m_bAttackReady;
	bool  m_bAttackStart;
	bool  m_bColorBlack;
	bool  m_bHitIceAllow;
	bool  m_bFreezeState;
	bool  m_bHitBroadSword;
	bool  m_bHitGrenade;
	D3DXVECTOR3 m_vSavaPos;

	CGameObject* m_pMainHp;
	CGameObject* m_pServeHP;
	CGameObject* m_pHPBack;
	CGameObject* m_pMiniMapObj;
	CGameObject* m_pFreezeState;

};

