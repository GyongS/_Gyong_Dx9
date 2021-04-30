#pragma once
#include "GameObject.h"
class CBrownWood :	public CGameObject
{
public:
	enum BrownWood_STATE
	{
		IDLE,
		WALK,
		ATTACK,
		DEAD,
		HIT,
		HIT2,
		END
	};
public:
	CBrownWood();
	virtual ~CBrownWood();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Late_Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void Set_CheckGround(bool _bCheck) { m_bCheckGround =  _bCheck; }
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
	void Check_LeftRight();
	void Monster_Pattern();
	void Set_State(BrownWood_STATE _eState) { m_eCurState = _eState; }
private:
	void IDLE_STATE();
	void WALK_STATE();
	void ATTACK_STATE();
	void HIT_ILSEOM_STATE();
	void FREEZE_STATE();
	void HIT_BROAD_SWORD();
	void HIT_GRENADE();

private:
	BrownWood_STATE m_eCurState;
	BrownWood_STATE m_ePreState;

private:
	int   m_iDir;
	float m_fDelay;
	bool  m_bAnimisPlaying;
	bool  m_bHitCheck;
	bool  m_bCreateCol;
	bool  m_bCheckGround;
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

