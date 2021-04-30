#pragma once
#include "GameObject.h"
class CClegane : public CGameObject
{
public:
	enum Clegane_STATE
	{
		INTRO,
		IDLE,
		EARTHQUAKE_READY,
		EARTHQUAKE_JUMP,
		EARTHQUAKE_ATTACK,
		EARTHQUAKE_HIT,
		STAMPING_NORMAL,
		STAMPING_READY,
		STAMPING_JUMP,
		STAMPING_HIT,
		WHIRLWIND_READY,
		WHIRLWIND_LOOP,
		DEAD,
		END
	};
public:
	CClegane();
	virtual ~CClegane();

	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void Set_bColorBlack(bool _bCheck) { m_bColorBlack = _bCheck; }
	void Set_HitIceAllow(bool _bCheck) { m_bHitIceAllow = _bCheck; }
	void Set_FreezeStage(bool _bCheck) { m_bFreezeState = _bCheck; }
	void Set_HitBroadSword(bool _bCheck) { m_bHitBroadSword = _bCheck; }
	void Set_HitGrenade(bool _bCheck) { m_bHitGrenade = _bCheck; }
	void Set_Damage(int _InDamage);
	Clegane_STATE& Get_State() { return m_eCurState; }
public:
	bool& Get_HitIceAllow() { return m_bHitIceAllow; }
	bool& Get_FreezeState() { return m_bFreezeState; }
	bool& Get_HitGrenade() { return m_bHitGrenade; }
	bool& Get_HitBroadSword() { return m_bHitBroadSword; }
private:
	void Change_State();
	void Check_LeftRight();
	void Boss_Pattern();
	void Set_State(Clegane_STATE _eState) { m_eCurState = _eState; }
	void Wall_Check();
private:
	void INTRO_STATE();
	void IDLE_STATE();
	void EARTHQUAKE_READY_STATE();
	void EARTHQUAKE_JUMP_STATE();
	void EARTHQUAKE_ATTACK_STATE();
	void EARTHQUAKE_HIT_STATE();
	void STAMPING_NORMAL_STATE();
	void STAMPING_READY_STATE();
	void STAMPING_JUMP_STATE();
	void STAMPING_HIT_STATE();
	void WHIRLWIND_READY_STATE();
	void WHIRLWIND_LOOP_STATE();
	void DEAD_STATE();
	void HIT_ILSEOM_STATE();
	void FREEZE_STATE();
	void HIT_BROAD_SWORD();
	void HIT_GRENADE();
private:
	Clegane_STATE m_eCurState;
	Clegane_STATE m_ePreState;

private:
	D3DXVECTOR3 m_vStartPos;
	D3DXVECTOR3 m_vEndPos;
	D3DXVECTOR3 m_vSavePos;
private:
	float m_fXSpeed;
	float m_fYSpeed;
	float m_fGravity;
	float m_fEndTime;
	float m_fMaxHeight;
	float m_fHeight;
	float m_fEndHeight;
	float m_fMaxTime;
	float m_fSoundDelay;
private:
	int   m_iSelect;
	int   m_iSpawnCnt;
	int   m_iPreSelect;
private:
	float m_fDist;
	float m_fDelay;
	float m_fTimer;

private:
	bool  m_bAnimisPlaying;
	bool  m_bHitCheck;
	bool  m_bCreateCol;
	bool  m_bCreateWave;
	bool  m_bNormalStamping;	
	bool  m_bJumpStamping;
	bool  m_bAttack;
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

