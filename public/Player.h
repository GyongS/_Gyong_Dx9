#pragma once
#include "GameObject.h"
class CPlayer : public CGameObject
{
public:
	enum PlayerType
	{
		TYPE_DEFAULT,
		TYPE_DEFAULT_NOBONE,
		TYPE_DEFAULT_NOHEAD,
		TYPE_PRISONER,
		TYPE_SAMURAI,
		TYPE_END,
	};

	enum Default_State
	{
		DEFAULT_RESPAWN,
		DEFAULT_IDLE,
		DEFAULT_WALK,
		DEFAULT_ATTACK_A,
		DEFAULT_ATTACK_B,
		DEFAULT_JUMP,
		DEFAULT_DOUBLEJUMP,
		DEFAULT_FALL,
		DEFAULT_DASH,
		DEFAULT_SKILL_A,
		DEFAULT_SWITCH,
		DEFAULT_DEAD,
		DEFAULT_END
	};

	enum Default_Nobone_State
	{
		DEFAULT_NOBONE_IDLE,
		DEFAULT_NOBONE_WALK,
		DEFAULT_NOBONE_JUMP,
		DEFAULT_NOBONE_FALL,
		DEFAULT_NOBONE_DASH,
		DEFAULT_NOBONE_GETBONE,
		DEFAULT_NOBONE_END

	};

	enum Default_NoHead_State
	{
		DEFAULT_NOHEAD_IDLE,
		DEFAULT_NOHEAD_WALK,
		DEFAULT_NOHEAD_ATTACK_A,
		DEFAULT_NOHEAD_ATTACK_B,
		DEFAULT_NOHEAD_JUMP,
		DEFAULT_NOHEAD_FALL,
		DEFAULT_NOHEAD_DASH,
		DEFAULT_NOHEAD_DEAD,
		DEFAULT_NOHEAD_END
	};

	enum Prisoner_State
	{
		PRISONER_IDLE,
		PRISONER_WALK,
		PRISONER_ATTACK_A,
		PRISONER_ATTACK_B,
		PRISONER_ATTACK_C,
		PRISONER_ATTACK_D,
		PRISONER_ATTACK_E,
		PRISONER_DASH,
		PRISONER_JUMP,
		PRISONER_DOUBLE_JUMP,
		PRISONER_JUMPATTACK,
		PRISONER_FALL,
		PRISONER_SKILL_A,
		PRISONER_SKILL_B_CHARGE,
		PRISONER_SKILL_B_END,
		PRISONER_SKILL_B_SHOT,
		PRISONER_SKILL_B_START,
		PRISONER_SKILL_C,
		PRISONER_SWITCH,
		PRISONER_DEAD,
		PRISONER_END
	};

	enum Samurai_State
	{
		SAMURAI_IDLE,
		SAMURAI_WALK,
		SAMURAI_ATTACK_A,
		SAMURAI_ATTACK_B,
		SAMURAI_ATTACK_C,
		SAMURAI_DASH,
		SAMURAI_JUMP,
		SAMURAI_JUMP_ATTACK,
		SAMURAI_DOUBLE_JUMP,
		SAMURAI_FALL,
		SAMURAI_PARRY,
		SAMURAI_PARRY_ATTACK,
		SAMURAI_SKILL_A,
		SAMURAI_SKILL_ILSEOM,
		SAMURAI_SKILL_C,
		SAMURAI_SWITCH,
		SAMURAI_DEAD,
		SAMURAI_END
	};

public:
	explicit CPlayer();
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Late_Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	PlayerType& Get_Type() { return m_eCurType; }
	PlayerType& Get_SaveType() { return m_eSaveType; }

	int& Get_Gold() { return m_tStat.iGold; }
	int& Get_DarkQuartz() { return m_tStat.iDarkQuartz; }
	int& Get_HeadCnt() { return m_iHeadCnt; }
	bool& Get_DropItem() { return m_bDropItem; }
	bool& Get_bColCheck() { return m_bCheckCol; }
	bool& Get_bGetBone() { return m_bGetBone; }	
	bool& Get_InPortal() { return m_bInPortal; }
	bool& Get_InItemShop() { return m_bInItemShop; }
	bool& Get_bHit_ILSeom() { return m_bHit_ILSeom; }
	bool& Get_bHitCheck() { return m_bHit; }
	BYTE& Get_SkillCnt() { return m_iSkillCnt; }

public:
	void Set_InPortal(bool _bCheck) { m_bInPortal = _bCheck; }
	void Set_InItemShop(bool _bCheck) { m_bInItemShop = _bCheck; }
	void Set_Damage(int InDamage) { m_tStat.fCurHP -= InDamage; }
	void Set_State(Default_State eState) { m_eCurDefaultState = eState; }
	void Set_Type(PlayerType eType) { m_eCurType = eType; }
	void Set_GroundCheck(bool _bCheck) { m_bGround = _bCheck; }
	void Set_JumpCheck(bool _bCheck) { m_bJump = _bCheck; }
	void Set_DoubleJumpCheck(bool _bCheck) { m_bDoubleJump = _bCheck; }
	void Set_JumpTime(float _time) { m_fJumpTime = _time; }
	void Set_JumpCount(int _Count) { m_iJumpCnt = _Count; }
	void Set_Gold(int _InGold) { m_tStat.iGold += _InGold; }
	void Set_Get_DarkQuartz(int _InDarkQuartz) { m_tStat.iDarkQuartz += _InDarkQuartz; }
	void Set_SaveType(PlayerType _eType) { m_eSaveType = _eType; }
	void Set_DropItem(bool _bCheck) { m_bDropItem = _bCheck; }
	void Set_HeadCnt(int _Count) { m_iHeadCnt += _Count; }	
	void Set_HitILSeom(bool _bCheck) { m_bHit_ILSeom = _bCheck; }
	void Set_HitCheck(bool _bCheck) { m_bHit = _bCheck; }
	void Set_PickUpHead(bool _bCheck) { m_bPickUpHead = _bCheck; }
public:
	static CPlayer* Create();
	void Free();

private:
	void Tutorial_Stage();
	void NextStage();	
	void Change_Type();
	void Change_AnimState();	
	void Update_SkillCoolDown();
	void OffSet();
	
private:
	void STATE_GETBONE();
	void STATE_WALK();
	void STATE_ATTACK();
	void STATE_JUMP();
	void STATE_DASH();
	void STATE_SWAP();
	void STATE_SKILL_A();
	void STATE_SKILL_S();
	void STATE_SKILL_D();
	
private:
	BYTE m_iSkillCnt;

private:
	int m_iComboCnt;
	int m_iJumpCnt;
	int m_iMainFaceIconNum;
	int m_iSwitchFaceIconNum;	
	int m_iHeadCnt;
	int m_iColCnt;
private:
	float m_fJumpForce;
	float m_fJumpTime;
	float m_fJumpY;
	float m_fGravity;

	float m_fDashSpeed;
	float m_fDashTime;
	float m_fTime;
	float m_fILSEOM_Delay;
	float m_fInvincibletime;

private:
	bool m_bWalk;
	bool m_bJump;
	bool m_bDoubleJump;
	bool m_bDash;
	bool m_bSwap;
	bool m_bGround;
	bool m_bPlayingAnim;
	bool m_bPlayingBroadSword;
	bool m_bSkill_B_Shot;
	bool m_bStartDashEffect;
	bool m_bStartAttack;
	bool m_bSpawnEffect;
	bool m_bDropItem;
	bool m_bCheckCol;
	bool m_bGetBone;
	bool m_bInPortal;
	bool m_bILSEOM_Start;
	bool m_bILSEOM_End;
	bool m_bHit_ILSeom;
	bool m_bUse_SKILL_A;
	bool m_bUse_SKILL_S;
	bool m_bUse_SKILL_D;
	bool m_bUse_Swap;
	bool m_bHit;
	bool m_bInItemShop;
	bool m_bPickUpHead;
private:
	PlayerType  m_eCurType;
	PlayerType  m_ePreType;
	PlayerType  m_eSaveType;

	Default_State m_eCurDefaultState;
	Default_State m_ePreDefaultState;

	Default_Nobone_State m_eCurDefaultNoboneState;
	Default_Nobone_State m_ePreDefaultNoboneState;

	Default_NoHead_State m_eCurDefaultNoHeadState;
	Default_NoHead_State m_ePreDefaultNoHeadState;

	Prisoner_State m_eCurPrisonerState;
	Prisoner_State m_ePrePrisonerState;

	Samurai_State m_eCurSamuraiState;
	Samurai_State m_ePreSamuraiState;

};

